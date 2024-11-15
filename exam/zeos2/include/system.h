/*
 * system.h - Capçalera del mòdul principal del sistema operatiu
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <types.h>

extern int zeos_ticks;
extern TSS         tss;
extern Descriptor* gdt;

void write_msr(unsigned int msr, unsigned int value);
unsigned long read_ebp();
unsigned long read_ebx();
unsigned long read_edi();
unsigned long read_esi();
void write_ebx(unsigned long value);
void write_edi(unsigned long value);
void write_esi(unsigned long value);

#endif  /* __SYSTEM_H__ */
