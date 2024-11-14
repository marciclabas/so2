/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>
#include <utils.h>
#include <io.h>
#include <mm.h>
#include <mm_address.h>
#include <sched.h>
#include <errs.h>
#include <system.h>
#include <libc.h>

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -EBADF;
  if (permissions!=ESCRIPTURA) return -EACCES;
  return 0;
}

int sys_ni_syscall()
{
	return -ENOSYS;
}

int sys_getpid() {
	return current()->PID;
}

void copy_pt_entry(
  task_struct * from, int idx_from,
  task_struct * to, int idx_to
) {
  set_ss_pag(get_PT(to), idx_to, get_frame(get_PT(from), idx_from));
}

int allocate_user_data_pages(task_struct * task) {
  for (int i = 0; i < NUM_PAG_DATA; i++) {
    int page = PAG_LOG_INIT_DATA + i;
    unsigned int frame = alloc_frame();
    if (frame == -1) {
      free_user_pages(task);
      return -1;
    }
    set_ss_pag(get_PT(task), page, frame);
  }
  return 0;
}

int ret_from_fork() {
  return 0;
}

int sys_fork() {

  if (list_empty(&freequeue))
    return -1;

  list_head* head = list_pop(&freequeue);
  task_struct* child = list_head_to_task_struct(head);
  task_struct* parent = current();

  copy_data(parent, child, sizeof(task_union));

  allocate_DIR(child);
  
  if (allocate_user_data_pages(child) < 0) {
    list_add_tail(head, &freequeue); // give back the PCB
    return -1;
  }

  // copy kernel pages
  for (int page = 0; page < NUM_PAG_KERNEL; page++)
    copy_pt_entry(parent, page, child, page);
  
  // copy user code pages
  for (int page = PAG_LOG_INIT_CODE; page < PAG_LOG_INIT_CODE + NUM_PAG_CODE; page++)
    copy_pt_entry(parent, page, child, page);

  // append child data pages to parent
  for (int i = 0; i < NUM_PAG_DATA; i++)
    copy_pt_entry(
      child, PAG_LOG_INIT_DATA + i,
      parent, PAG_LOG_INIT_CODE + NUM_PAG_CODE + i // after user code pages
    );

  // copy user data pages to temporal entries
  #define TEMPORAL_START ((PAG_LOG_INIT_CODE+NUM_PAG_CODE)*PAGE_SIZE)
  copy_data(L_USER_START, TEMPORAL_START, NUM_PAG_DATA*PAGE_SIZE);

  // remove temporal entries from parent
  for (int i = 0; i < NUM_PAG_DATA; i++)
    del_ss_pag(get_PT(parent), PAG_LOG_INIT_CODE + NUM_PAG_CODE + i);

  // flush TLB
  set_cr3(get_DIR(parent));

  child->PID = new_pid();
  child->state = ST_READY;
  child->kernel_esp += (DWord) child - (DWord) parent; // offset from the base is the same
  // kernel_esp didn't account for the new context, so we need to add it
  child->kernel_esp -= 19 * sizeof(DWord); // 17 context + 1 ebp + 1 ret_from_fork
  DWord * esp = (DWord*) child->kernel_esp;
  esp[0] = 0; // fake ebp
  esp[1] = (DWord) &ret_from_fork;

  INIT_LIST_HEAD(&child->children);
  child->parent = parent;
  list_add_tail(&child->child_anchor, &parent->children);

  list_add_tail(&child->list, &readyqueue);
  return child->PID;
}

void sys_exit() {
  task_struct * curr = current();
  update_process_state_rr(curr, &freequeue);
  sched_next_rr();
  list_del(&curr->child_anchor);

  list_head* it;
  list_for_each(it, &curr->children) {
    task_struct * child = list_head_to_child(it);
    list_add_tail(&idle_task->children, &child->child_anchor);
  }
  
  page_table_entry * pt = get_PT(curr);
  for (int i = 0; i < TOTAL_PAGES; i++) {
    if (pt[i].bits.present) {
      free_frame(get_frame(pt, i));
      del_ss_pag(pt, i);
    }
  }
}

int sys_gettime() {
  return zeos_ticks;
}

int check_params(int fd, char * buffer, int size) {
  if (fd != 1) return -EBADF;
  if (buffer == NULL) return -EFAULT;
  if (size < 0) return -EINVAL;
  return 0;
}

int sys_write(int fd, char * buffer, int size) {
  int status = check_params(fd, buffer, size);
  if (status < 0) return status;

  char sys_buffer[size];
  int copy_status = copy_from_user(buffer, sys_buffer, size);
  if (copy_status < 0) return copy_status;

  return sys_write_console(sys_buffer, size);
}

extern list_head blocked;

void sys_block() {
  task_struct * curr = current();
  if (curr->pending_unblocks > 0)
    curr->pending_unblocks--;
  else {
    curr->state = ST_BLOCKED;
    list_add_tail(&curr->list, &blocked);
    sched_next_rr();
  }
}

task_struct* find_child(list_head* children, int pid) {
  list_head * it;
  list_for_each(it, children) {
    task_struct * child = list_head_to_child(it);
    if (child->PID == pid)
      return child;
  }
  return NULL;
}

int sys_unblock(int pid) {
  task_struct * curr = current();

  task_struct* child = find_child(&curr->children, pid);
  if (child == NULL)
    return -1;

  if (child->state == ST_BLOCKED) {
    list_del(&child->list);
    list_add_tail(&child->list, &readyqueue);
    child->state = ST_READY;
  }
  else {
    child->pending_unblocks++;
  }

  return 0;
}