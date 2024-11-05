#include <libc.h>
#include <io.h>

int pid;

void sleep(int ticks) {
  int t0 = gettime();
  while (gettime() < t0 + ticks);
}


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  int pid = fork();
  if (pid == 0) {
    printf("Blocking child (pid=%d)\n", getpid());
    block();
    printf("Unblocked\n");
    sleep(1000);
    printf("Exiting child\n");
    exit();
  } else {
    printf("Parent. Child pid=%d\n", pid);
    sleep(100);
    printf("Unblocking child\n");
    int ok = unblock(pid);
    printf("Unblock -> %d\n", ok);
    while(1);
  }
}
