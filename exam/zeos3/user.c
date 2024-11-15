#include <libc.h>
#include <io.h>

void sleep(int ticks) {
  int t0 = gettime();
  while (gettime() < t0 + ticks);
}

void test_block() {
  int pid = fork();
  if (pid == 0) {
    printf("Blocking child (pid=%d)\n", getpid());
    block();
    printf("Unblocked\n");
    sleep(1000);
    printf("Exiting child\n");
    exit(0);
  } else {
    printf("Parent. Child pid=%d\n", pid);
    sleep(100);
    printf("Unblocking child\n");
    int ok = unblock(pid);
    printf("Unblock -> %d\n", ok);
    sleep(1000);
    printf("Exiting parent\n");
    exit(0);
  }
}

void test_waitpid_immediate() {
  int pid = fork();
  if (pid == 0) {
    printf("Child (pid=%d) exiting\n", getpid());
    exit(32);
  } else {
    sleep(1000);
    printf("Parent waiting for child %d\n", pid);
    int status;
    int ok = waitpid(pid, &status);
    printf("Waitpid -> %d, status=%d\n", ok, status);
  }
}

void test_waitpid_delayed() {
  int pid = fork();
  if (pid == 0) {
    sleep(1000);
    printf("Child (pid=%d) exiting\n", getpid());
    exit(32);
  } else {
    printf("Parent waiting for child %d\n", pid);
    int status;
    int ok = waitpid(pid, &status);
    printf("Waitpid -> %d, status=%d\n", ok, status);
  }
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
  // test_waitpid_immediate();
  test_waitpid_delayed();
  while(1);
}
