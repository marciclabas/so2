/*
 * interrupt.h - Definició de les diferents rutines de tractament d'exepcions
 */

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <types.h>

#define IDT_ENTRIES 256

#define BUFFER_SIZE 8

typedef struct buffer_circular {
  char buff[BUFFER_SIZE];
  int start;
  int end;
} buffer_circular;

extern buffer_circular keyboard_buffer;

extern Gate idt[IDT_ENTRIES];
extern Register idtR;

void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL);
void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL);

void setIdt();
void keyboard_handler();
void clock_handler();
void page_fault_handler2();
void system_call_handler();
void syscall_handler_sysenter();
void buffer_add(char c, buffer_circular* buffer);
char buffer_pop(buffer_circular* buffer);
void init_buff(buffer_circular* buffer);



#endif  /* __INTERRUPT_H__ */
