/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <io.h>
#include <system.h>
#include <libc.h>

#include <zeos_interrupt.h>

Gate idt[IDT_ENTRIES];
Register    idtR;


buffer_circular keyboard_buffer;

void init_buff(buffer_circular* buff){
  buff->start = 0;
  buff->end = 0;
}

void buffer_add(char c, buffer_circular* buffer){
  buffer->buff[buffer->end] = c;
  buffer->end = (buffer->end + 1) % BUFFER_SIZE;
  if(buffer->start == buffer->end) buffer->start = (buffer->start + 1) % BUFFER_SIZE;
}

char buffer_pop(buffer_circular* buffer){
  if(buffer->end == buffer->start) return -1;
  char ret = buffer->buff[buffer->start];
  buffer->start = (buffer->start + 1) % BUFFER_SIZE;
  return ret;
}

char char_map[] =
{
  '\0','\0','1','2','3','4','5','6',
  '7','8','9','0','\'','�','\0','\0',
  'q','w','e','r','t','y','u','i',
  'o','p','`','+','\0','\0','a','s',
  'd','f','g','h','j','k','l','�',
  '\0','�','\0','�','z','x','c','v',
  'b','n','m',',','.','-','\0','*',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0','\0','\0','\0','\0','\0','7',
  '8','9','-','4','5','6','+','1',
  '2','3','0','\0','\0','\0','<','\0',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0'
};



void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void keyboard_routine() {
  unsigned char inp = inb(0x60);

  int keyup = inp & 0x80;
  char c = char_map[inp & 0x7F];
  
  if (!keyup) {
    int ret = key_unblock(c);
    if (!ret)
      buffer_add(c, &keyboard_buffer);
  }
}

void clock_routine() {
  getkey_blocked_reduce_time();
  zeos_ticks++;
  zeos_show_clock();
  schedule();
  
  
}

void page_fault_routine2(int access_addr, int instr_addr) {
  printf("Page fault at address: 0x%x, accessing 0x%x\n", instr_addr, access_addr);
  while (1);
}

void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;
  
  set_handlers();

  /* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */
  setInterruptHandler(33, keyboard_handler, 0);
  setInterruptHandler(32, clock_handler, 0);
  setInterruptHandler(14, page_fault_handler2, 0);
  setTrapHandler(0x80, system_call_handler, 3);

  set_idt_reg(&idtR);
}

