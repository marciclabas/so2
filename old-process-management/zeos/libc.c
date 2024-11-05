/*
 * libc.c 
 */

#include <libc.h>
#include <types.h>
#include <errs.h>
#include <stdarg.h>

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

void reverse(char * s, int size) {
  for (int i = 0; i < size / 2; i++) {
    char temp = s[i];
    s[i] = s[size-i-1];
    s[size-i-1] = temp;
  }
}

void int2base(int a, char *b, int base) {
  int i = 0;

  if (a == 0) {
    b[0] = '0';
    b[1] = '\0';
    return;
  }

  while (a > 0) {
    int remainder = a % base;
    b[i] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'A';
    a = a / base;
    i++;
  }

  reverse(b, i);
  b[i] = '\0';
}

int strlen(char *a) {
  int i = 0;
  while (a[i]!=0) i++;
  return i;
}

void printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  char buffer[33];  // Buffer to hold converted values (up to 32 bits for binary)
  int i = 0;
  
  while (format[i] != '\0') {
    if (format[i] == '%' && format[i + 1] == 'd') {
      int value = va_arg(args, int);
      int2base(value, buffer, 10);
      write(1, buffer, strlen(buffer));
      i += 2;
    } else if (format[i] == '%' && format[i + 1] == 'b') {
      int value = va_arg(args, int);
      int2base(value, buffer, 2); 
      write(1, "0b", 2);
      write(1, buffer, strlen(buffer));
      i += 2;
    } else if (format[i] == '%' && format[i + 1] == 'x') {
      int value = va_arg(args, int);
      int2base(value, buffer, 16);
      write(1, "0x", 2);
      write(1, buffer, strlen(buffer));
      i += 2;
    } else {
      char temp[2] = {format[i], '\0'};
      write(1, temp, 1);
      i++;
    }
  }
  va_end(args);
}