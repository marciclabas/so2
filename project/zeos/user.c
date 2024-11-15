#include <libc.h>
#include <io.h>
//#include <stddef.h>
void sleep(int ticks) {
  int t0 = gettime();
  while (gettime() < t0 + ticks);
}


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
  printf("hola");
  int z = gotoXY(0,0);
  if(z==1)printf("adeu");
  
  int p = changeColor(0x4, 0x1);
  
  int k = gotoXY(40,10);
   if(k==1) printf("adeu");
   char b = 'i';
   int i = clrscr(&b);
  while(1){

  }
}
