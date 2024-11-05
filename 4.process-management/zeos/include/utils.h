#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>

void copy_data(void *start, void *dest, int size);
int copy_from_user(void *start, void *dest, int size);
int copy_to_user(void *start, void *dest, int size);

#define VERIFY_READ	0
#define VERIFY_WRITE	1
int access_ok(int type, const void *addr, unsigned long size);

#define min(a,b)	(a<b?a:b)

unsigned long get_ticks(void);

void reverse(char * s, int size);
void int2base(int a, char *b, int base);
void inner_printf(void (*print)(const char*s), char *fmt, va_list args);

#endif
