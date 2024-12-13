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
int COLOR = 2; 
//bg: 8 7 6 5 Fg: 4 3 2 1
//      r g b       r g b

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
  Word ch = (Word) (c & 0x00FF) | (COLOR << 8);
  Word *screen = (Word *)0xb8000;
  screen[(y * NUM_COLUMNS + x)] = ch;
  if (++x >= NUM_COLUMNS)
  {
  x = 0;
  y=(y+1)%NUM_ROWS;
  }
  }
}

int sys_clrscr(char* b){
  if(b != NULL) {
    int ok = access_ok(VERIFY_READ, b, 2*NUM_COLUMNS*NUM_ROWS);
    Word *screen = (Word *)0xb8000;
    copy_data(b, screen, 2*NUM_ROWS*NUM_COLUMNS);
  }
  else {
    for(int i = 0; i<NUM_ROWS; i++)
      for(int j = 0; j<NUM_COLUMNS; j++)
        printc(' ');
    printf("hola");
  }
  return 1;
}


int sys_changeColor(int fg, int bg){
  int fgc = fg % 8;
  int bgc = bg % 8;
  COLOR = (bgc << 4) + fgc;
  return 1;
}

int sys_gotoXY(int px,int py){
  x = px % NUM_COLUMNS;
  y = py % NUM_ROWS;
  return 1;
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
