/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}


int sys_write(int fd, char * buffer, int size) {
  int fd_status = check_fd(fd, ESCRIPTURA);
  if (fd_status < 0) return fd_status;
  if (buffer == NULL) return -14; /*EFAULT*/
  if (size < 0) return -22; /*EINVAL*/

  char sys_buffer[size];
  int status = copy_from_user(buffer, sys_buffer, size);
  if (status < 0) return status;

  return sys_write_console(sys_buffer, size);
}