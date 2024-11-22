/*
 * system.h - Capçalera del mòdul principal del sistema operatiu
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <types.h>
#include <list.h>

extern int zeos_ticks;
extern TSS         tss;
extern Descriptor* gdt;

void write_msr(unsigned int msr, unsigned int value);
int read_ebp();
int key_unblock(char c);

extern list_head key_blocked;

#endif  /* __SYSTEM_H__ */
