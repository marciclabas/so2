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