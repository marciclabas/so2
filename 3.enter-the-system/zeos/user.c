#include <libc.h>
#include <io.h>

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  write(1, "Hello fellas\n", 13);
  perror();

  char * c = 0;
  c = 'n';
  write(1, &c, 1);

  while(1) {}
}
