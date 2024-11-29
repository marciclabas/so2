/*
 * io.h - Definició de l'entrada/sortida per pantalla en mode sistema
 */

#ifndef __IO_H__
#define __IO_H__

#include <types.h>

/** Screen functions **/
/**********************/

Byte inb (unsigned short port);
void printc(char c);
void printc_xy(Byte x, Byte y, char c);
void printk(const char *string);
void print_bits(int value, char * output, int size);
void print_hex(int value, char * output, int size);
void printf(const char *format, ...);
// int sys_gotoXY(int x, int y);
// int sys_changeColor(int fg, int bg);
// int sys_clrscr(char* b);

#endif  /* __IO_H__ */
