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
  int id = gettid();
  printf("..Thread: %d\n", id);
  sleep(1000);
  exit();
}

void test_threads(void * arg) {
  for (int i = 0; i < 2; i++) {
  int id = 2 + (int) arg;
    int ok = threadCreateWithStack(thread, 1, (void*) id);
    if (ok < 0)
      printf("Error creating thread %d\n", i);
    else
      printf(".Thread %d created\n", i);
  }
  sleep(2000);
  printf("em desperto 2\n");
  exit();
}

void test_threads_exit() {
  for (int i = 0; i < 3; i++) {
    int ok = threadCreateWithStack(test_threads, 1, (void*) i);
    if (ok < 0)
      printf("Error creating thread exit %d\n", i);
    else
      printf("Thread exit %d created\n", i);
  }
  sleep(4000);
  printf("em desperto\n");
  
  for (int i = 0; i < 3; i++) {
    int ok = threadCreateWithStack(test_threads, 1, (void*) i);
    if (ok < 0)
      printf("Error creating thread exit %d\n", i);
    else
      printf("Thread exit %d created\n", i);
  }
  //sleep(4000);
  printf("em desperto\n");
  
  exit();
}
