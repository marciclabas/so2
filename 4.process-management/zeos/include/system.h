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
void init_msrs();

#endif  /* __SYSTEM_H__ */
