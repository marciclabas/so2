#include <libc.h>
#include <test.h>
#include <joc.h>



void sleep(int ticks) {
  int t0 = gettime();
  while (gettime() < t0 + ticks);
}

void test_dyn_mem() {
  char * reg = memRegGet(3);
  int size = 4096*3 - 4;

  for (int i = 0; i < size; i++)
    reg[i] = 'a';

  for (int i = 0; i < size; i++) {
    if (reg[i] != 'a') {
      printf("Error: reg[%d] = %c\n", i, reg[i]);
    }
  }

  printf("Freeing memory\n");
  memRegDel(reg);
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
  //test_screen();
  //test_threads();
  //test_dyn_mem();
  //test_exit();
  //test_threads();
  //test_threads_exit();
  //test_sem();
  game();
  //printf("Tornem al main..........\n");
  while(1);
}
