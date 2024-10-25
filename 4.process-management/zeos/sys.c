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

int sys_getpid()
{
	return current()->PID;
}

int sys_fork() {
  if (list_empty(&freequeue))
    return -1;

  struct list_head * head = list_first(&freequeue);
  list_del(head);
  struct task_struct * child = list_head_to_task_struct(head);

  struct task_struct * parent = current();
  copy_data(parent, child, KERNEL_STACK_SIZE);

  allocate_DIR(child);
  
  for (int i = 0; i < NUM_PAG_DATA; i++) {
    if (get_PT(parent)[i].bits.present) {
      unsigned int frame = alloc_frame();
      if (frame == -1) {
        free_user_pages(child);
        return -1;
      }
      
      set_ss_pag(get_PT(child->dir_pages_baseAddr), i, frame);
    }
  }
}

void sys_exit()
{  
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
