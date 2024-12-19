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
#include <interrupt.h>

#define LECTURA 0
#define ESCRIPTURA 1

list_head key_blocked;

int key_unblock(char c) {
  if (list_empty(&key_blocked))
    return 0;

  list_head * head = list_pop(&key_blocked);
  task_struct * t = list_head_to_task_struct(head);
  t->key = c;
  list_add_tail(&t->list, &readyqueue);
  return 1;
}

void getkey_blocked_reduce_time(){
	list_head *it, *it2;
	list_for_each_safe(it, it2, &key_blocked){
		task_struct* task = list_head_to_task_struct(it);
		task->time_blocked--;
		
		if(task->time_blocked <= 0){//Si ha acabat el temps d'espera el desbloquejem
			/*list_del(it);
			task->key = -1;
			list_add_tail(it,&readyqueue);*/
			list_del(&task->list);
			task->key = -1;
			list_add_tail(&task->list,&readyqueue);
		}
	}
}

int sys_getkey(char* c, int timeout){
  char ret = buffer_pop(&keyboard_buffer);
  if(ret == -1) {
    list_add_tail(&current()->list, &key_blocked);
    current()->time_blocked = timeout;
    sched_next_rr();
    *c = current()->key;
    
    if(current()->key == -1){
  	return -1;
    }
  }
  else {
    *c = ret;
  }
  return 0;
}

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

int sys_gettid(){
	return current()->TID;
}

void copy_pt_entry(
  task_struct * from, int idx_from,
  task_struct * to, int idx_to
) {
  set_ss_pag(get_PT(to), idx_to, get_frame(get_PT(from), idx_from));
}

int allocate_user_data_pages(task_struct * task) {
  return alloc_pages(task, NUM_PAG_DATA, PAG_LOG_INIT_DATA);
}

int ret_from_fork() {
  return 0;
}

/** Copy user data pages from `parent` to `child`
 * 1. Copy `child` data entries to `parent` temporal entries
 * 2. Copy data
 * 3. Remove temporal entries
*/
void copy_user_data(task_struct* parent, task_struct* child) {
  for (int i = 0; i < NUM_PAG_DATA; i++)
    copy_pt_entry(
      child, PAG_LOG_INIT_DATA + i,
      parent, PAG_LOG_INIT_COPY + i
    );

  copy_data(L_USER_START, COPY_START, NUM_PAG_DATA*PAGE_SIZE);

  for (int i = 0; i < NUM_PAG_DATA; i++)
    del_ss_pag(get_PT(parent), PAG_LOG_INIT_COPY + i);
}

/** Copy heap from `t1` to `t2`
 * 1. For each heap page in `t1`, allocate a new frame in `t2`. Map it to a temporal entry in `t1`
 * 2. Copy data
 * 3. Remove temporal entries
*/
int copy_heap(task_struct* t1, task_struct* t2) {
  page_table_entry* pt1 = get_PT(t1);
  page_table_entry* pt2 = get_PT(t2);

  for (int i = 0; i < NUM_PAG_HEAP; i++) {
    int page = PAG_LOG_INIT_HEAP + i;
    
    if (pt1[page].bits.present) {
      int frame = alloc_frame();
      if (frame < 0)
        return -1;

      set_ss_pag(pt2, page, frame);
      int temp_page = PAG_LOG_INIT_TEMP + i;
      set_ss_pag(pt1, temp_page, frame);

      copy_data(page << 12, temp_page << 12, PAGE_SIZE);

      del_ss_pag(pt1, temp_page);
    }
  }
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

  // copy user data pages
  copy_user_data(parent, child);

  // copy heap
  if (copy_heap(parent, child) < 0) {
    list_add_tail(head, &freequeue); // give back the PCB
    free_user_pages(child);
    return -1;
  }

  // flush TLB
  set_cr3(get_DIR(parent));

  child->PID = new_pid();
  child->TID = new_tid();
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

  INIT_LIST_HEAD(&child->threads_created);
  child->thread_principal = 1;

  INIT_LIST_HEAD(&child->sems_created);


  list_add_tail(&child->list, &readyqueue);
  return child->PID;
}


void inner_exit(task_struct* curr){
  list_del(&curr->list);
  list_del(&curr->thread_anchor); //no cal comprovar que no es principal pq inner nomes el criden els threads pares...
  update_process_state_rr(curr, &freequeue);
  
  //eliminem recursivament els threads fills 
  if(!list_empty(&curr->threads_created)){
	list_head *it, *it2;
	list_for_each_safe(it, it2, &curr->threads_created) {
	task_struct* child = list_head_to_thread(it);
	inner_exit(child); 
  	}
  }
  
  //Posem que els fills son fills de IDLE -------------------------> Potser hem de fer que siguin fills del thread pare a no ser que aquest sigui el principal...		
  list_head *it;
  list_for_each(it, &curr->children) {
    task_struct * child = list_head_to_child(it);
    list_add_tail(&child->child_anchor, &idle_task->children);
  }
  
  //borrem el tros privat de la TP
  page_table_entry * pt = get_PT(curr); 
  for (int i = 0; i < curr->num_pages_thread; i++) {
    int page = curr->start_page_thread + i;
    if (pt[page].bits.present) {
      free_frame(get_frame(pt, page));
      del_ss_pag(pt, page);
    }
  }
  //eliminem tots els sems
  while(!list_empty(&curr->sems_created)){
    list_head* h = list_pop(&curr->sems_created);
    sem_destroy(h);
  }
}

void sys_exit() { //unica diferencia amb inner exit es q aqueest fa shednext abans de borrar la seva memoria i fa comprobacions per si es el thread principal
  task_struct * curr = current();
  update_process_state_rr(curr, &freequeue);

    if(curr->thread_principal == 0){
      list_del(&curr->thread_anchor);//si no es el principal segur que te thread pare
      if(!list_empty(&curr->threads_created)){
        list_head *it, *it2;
        task_struct* pare = curr->parent;
      list_for_each_safe(it, it2, &curr->threads_created) { //posem tots els threads fills que siguin threads del pare d'aquest
      //task_struct* child = list_head_to_thread(it);
      list_add(it, &pare->threads_created);
      }
    }

    //Posem que els fills son fills de IDLE 		
    if(!list_empty(&curr->children)){
        list_head *it;
        list_for_each(it, &curr->children) {
          task_struct * child = list_head_to_child(it);
          list_add_tail(&child->child_anchor, &idle_task->children);
        }
      }
    
    //eliminem espai privat TP 
    page_table_entry * pt = get_PT(curr);
    for (int i = 0; i < curr->num_pages_thread; i++) {
      int page = curr->start_page_thread + i;
      if (pt[page].bits.present) {
        free_frame(get_frame(pt, page));
        del_ss_pag(pt, page);
      }
    }
  }
  else { //si es el principal
    list_del(&curr->child_anchor);
    //eliminem recursivament els threads fills
    if(!list_empty(&curr->threads_created)){
    list_head *it, *it2;
    list_for_each_safe(it, it2, &curr->threads_created) {
    task_struct* child = list_head_to_thread(it);
      inner_exit(child); 
    }
    }

    //Posem que els fills son fills de IDLE 		
    if(!list_empty(&curr->children)){
        list_head *it;
      list_for_each(it, &curr->children) {
        task_struct * child = list_head_to_child(it);
        list_add_tail(&child->child_anchor, &idle_task->children);
      }
    }

    //Borra tota la TP
    page_table_entry * pt = get_PT(curr);
	  for (int i = 0; i < TOTAL_PAGES; i++) {
	    if (pt[i].bits.present) {
	      free_frame(get_frame(pt, i));
	      del_ss_pag(pt, i);
	    }
	  }
  }
  //eliminem els sems
  while(!list_empty(&curr->sems_created)){
    list_head* h = list_pop(&curr->sems_created);
    sem_destroy(h);
  }
  sched_next_rr();
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


int sys_threadCreateWithStack(
  void (*function)(void* arg), int N, void* parameter,
  void (*wrapper_fn)(void (*fn)(void* arg), void* arg)
) {

  if (N <= 0) {
    printf("Error creating thread: invalid number of pages\n");
    return -1;
  }

  if (list_empty(&freequeue)) {
    printf("Error creating thread: no PCBs available\n");
    return -1;
  }

  list_head* head = list_pop(&freequeue);
  task_struct* child = list_head_to_task_struct(head);
  unsigned long * child_stack = (unsigned long *) child;
  task_struct* parent = current();


  copy_data(parent, child, sizeof(task_union));

  int start_page = find_empty_pages(get_PT(child), N, PAG_LOG_INIT_HEAP);
  if (start_page == -1) {
    printf("Error creating thread: no pages available\n");
    list_add_tail(&child->list, &freequeue);
    return -1;
  }

  if (alloc_pages(child, N, start_page) == -1) {
    printf("Error creating thread: no frames available\n");
    list_add_tail(&child->list, &freequeue);
    return -1;
  }

  unsigned long stack_top = start_page << 12;
  
  unsigned long* user_stack = (unsigned long*) stack_top;

  int num_entries = PAGE_SIZE*N/sizeof(unsigned long);
  user_stack[num_entries-3] = 0;
  user_stack[num_entries-2] = function;
  user_stack[num_entries-1] = parameter;

  child->TID = new_tid();
  child->state = ST_READY;
  child->kernel_esp += (DWord) child - (DWord) parent; // offset from the base is the same
  // kernel_esp didn't account for the new context, so we need to add it
  child->kernel_esp -= 19 * sizeof(DWord); // 17 context + 1 ebp + 1 ret_from_fork

  DWord * esp = (DWord*) child->kernel_esp;
  esp[0] = 0; // fake ebp
  esp[1] = (DWord) &ret_from_fork;
  // user eip: 4-th from bottom
  esp[14] = (unsigned long) wrapper_fn;
  // user esp: 1-th from bottom
  esp[17] = &user_stack[num_entries-3];
  
  INIT_LIST_HEAD(&child->threads_created);
  INIT_LIST_HEAD(&child->children);
  INIT_LIST_HEAD(&child->sems_created);

  child->thread_principal = 0;
  child->thread_parent = parent;
  child->start_page_thread = start_page;
  child->num_pages_thread = N;

  list_add_tail(&child->thread_anchor, &parent->threads_created);
  
  list_add_tail(&child->list, &readyqueue);

  return child->TID;
}
