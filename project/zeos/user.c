#include <libc.h>
#include <test.h>

void sleep(int ticks) {
  int t0 = gettime();
  while (gettime() < t0 + ticks);
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
	// test_screen();
  test_threads();
  //test_exit();
  printf("Tornem al main..........\n");
  while(1);
}
