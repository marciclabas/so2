#include <libc.h>
#include <io.h>

int pid;
char buffer[10];

void print_time(int time) {
  itoa(time, buffer);
  write(1, "Time: ", 6);
  write(1, buffer, strlen(buffer));
  write(1, "\n", 1);
}

int last_time = -1e9;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  write(1, "\nFast hello!\n", 13);
  write_int(1, "\nSlow hello!\n", 13);

  last_time = gettime();
  print_time(last_time);

  while(1) {
    int time = gettime();
    if (time > last_time + 1000) {
      last_time = time;
      print_time(time);
    }
  }
}
