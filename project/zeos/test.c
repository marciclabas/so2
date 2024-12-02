#include <libc.h>

void test_fork() {
  int pid = fork();
  if (pid == 0) {
    printf("Child\n");
  } else {
    printf("Parent\n");
  }
}

void test_screen() {
	short screen[80][25];
	for (int i = 0; i < 80; i++)
		for (int j = 0; j < 25; j++) {
			screen[i][j] = '0' + (2 << 8);
		}
	clrscr(screen);
}

void thread(void * arg) {
  int id = (int) arg;
  printf("Thread: %d\n", id);
  exit();
}

void test_threads() {
  for (int i = 0; i < 10; i++) {
    int ok = threadCreateWithStack(thread, 1, (void*) i);
    if (ok < 0)
      printf("Error creating thread %d\n", i);
    else
      printf("Thread %d created\n", i);
  }
  exit();
}
