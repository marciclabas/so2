#include <libc.h>
#include <io.h>

int pid;


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  int pid = fork();
  if (pid == 0) {
    printf("Child\n");
  } else {
    printf("Parent. Child pid=%d\n", pid);
  }

  int last_time = gettime();

  while(1) {
    int time = gettime();
    if (time > last_time + 100) {
      last_time = time;
      printf("[pid=%d] Current Time: %d\n", getpid(), time);
    }
  }
}
