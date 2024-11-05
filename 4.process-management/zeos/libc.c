/*
 * libc.c 
 */

#include <stdarg.h>
#include <libc.h>
#include <types.h>
#include <errs.h>
#include <utils.h>

int errno;

const char * const err_msg[] = {
  [EBADF] = "Bad file number",
  [EACCES] = "Permission denied",
  [EFAULT] = "Bad address",
  [EINVAL] = "Invalid argument",
  [ENOSYS] = "Function not implemented",
};

void perror() {
  if (errno > 0) {
  write(1, "\nError: ", 9);
  write(1, err_msg[errno], strlen(err_msg[errno]));
  write(1, "\n", 1);
  }
  else {
  write(1, "\nNo error\n", 11);
  }
}

int strlen(char *a) {
  int i = 0;
  while (a[i]!=0) i++;
  return i;
}

void print(char *a) {
  write(1, a, strlen(a));
}

void printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  inner_printf(print, fmt, args);
  va_end(args);
}