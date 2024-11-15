/*
 * io.c - 
 */

#include <stdarg.h>
#include <io.h>
#include <types.h>
#include <utils.h>

/**************/
/** Screen  ***/
/**************/

#define NUM_COLUMNS 80
#define NUM_ROWS    25

Byte x, y=19;

/* Read a byte from 'port' */
Byte inb (unsigned short port)
{
  Byte v;

  __asm__ __volatile__ ("inb %w1,%0":"=a" (v):"Nd" (port));
  return v;
}

void printc(char c)
{
   __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c)); /* Magic BOCHS debug: writes 'c' to port 0xe9 */
  if (c=='\n')
  {
  x = 0;
  y=(y+1)%NUM_ROWS;
  }
  else
  {
  Word ch = (Word) (c & 0x00FF) | 0x0200;
  Word *screen = (Word *)0xb8000;
  screen[(y * NUM_COLUMNS + x)] = ch;
  if (++x >= NUM_COLUMNS)
  {
  x = 0;
  y=(y+1)%NUM_ROWS;
  }
  }
}

void printc_xy(Byte mx, Byte my, char c)
{
  Byte cx, cy;
  cx=x;
  cy=y;
  x=mx;
  y=my;
  printc(c);
  x=cx;
  y=cy;
}

void printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  inner_printf(printk, format, args);
  va_end(args);
}

void printk(const char *string) {
  for (int i = 0; string[i]; i++)
    printc(string[i]);
}
