#include <libc.h>
#include <io.h>

void sleep(int ticks) {
  int t0 = gettime();
  while (gettime() < t0 + ticks);
}


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
  
  while(1){
    char buff[3] = {'0', '\n', '\0'};
    printf("Waiting\n");
    int res = getkey(buff,2);
    printf("Received");
    if(res != -1) printf(buff);
    printf("Done\n");
  }
}
