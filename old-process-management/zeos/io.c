/*
 * io.c - 
 */

#include <io.h>
#include <stdarg.h>
#include <types.h>

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

void printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  char buffer[33];
  int i = 0;
  
  while (format[i] != '\0') {
    if (format[i] == '%' && format[i + 1] == 'd') {
      int value = va_arg(args, int);
      int2base(value, buffer, 10);
      printk(buffer);
      i += 2;
    } else if (format[i] == '%' && format[i + 1] == 'b') {
      int value = va_arg(args, int);
      int2base(value, buffer, 2);
      printk("0b");
      printk(buffer);
      i += 2;
    } else if (format[i] == '%' && format[i + 1] == 'x') {
      int value = va_arg(args, int);
      printk("0x");
      int2base(value, buffer, 16);
      printk(buffer);
      i += 2;
    } else {
      printc(format[i]);
      i++;
    }
  }
  va_end(args);
}

void printk(char *string) {
  for (int i = 0; string[i]; i++)
    printc(string[i]);
}
