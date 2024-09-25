/*
 * libc.c 
 */

#include <libc.h>
#include <types.h>
#include <errs.h>

int errno;

const char * const err_msg[] = {
  [EBADF] = "Bad file number",
  [EACCES] = "Permission denied",
  [EFAULT] = "Bad address",
  [EINVAL] = "Invalid argument",
  [ENOSYS] = "Function not implemented",
};

void perror() {
  if (errno < 0) {
    write(1, "\nError: ", 9);
    write(1, err_msg[errno], strlen(err_msg[errno]));
  }
  else {
    write(1, "No error\n", 10);
  }
}

void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}

