/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

#include <stats.h>

int write(int fd, const char *buffer, int size);
int getkey(char* c, int timeout);

void itoa(int a, char *b);
void int2base(int a, char *b, int base);

int strlen(const char *a);

int getpid();

int fork();

void printf(const char *format, ...);

int gettime();
void perror();
void exit();

void block();
int unblock(int pid);

int clrscr(char* b);
int changeColor(int fg, int bg);
int gotoXY(int px,int py);

int threadCreateWithStack( void (*function)(void* arg), int N, void* parameter);

#endif  /* __LIBC_H__ */
