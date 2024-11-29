#include <libc.h>
#include <io.h>
//#include <stddef.h>
void sleep(int ticks) {
  int t0 = gettime();
  while (gettime() < t0 + ticks);
}

void test_getkey() {
	char b[10];
	int time = 1000;
  	printf("Posa algo al buffer corre");
	for(int i = 0; i<100000000; i++) if (i%10000000 == 0) printf(".");
	printf("\n");
	
	/*printf("Cridem getkey-> ");
	int ret = getkey(b, time);
  	if(ret != -1) printf("Fet:  %s\n", b);
  	else printf("S'ha acabat el temps\n");*/
  	
  	int pid = fork();
  	if(pid == 0){
  		char b[10];
		int time = 1000;
		printf("cridem a getkey fill\n");
  		int ret = getkey(b, time);
  		if(ret != -1) printf("Fet:  %s\n", b);
  		else printf("S'ha acabat el temps fill\n");
  	}
  	else{
  		char b[10];
		int time = 2000;
		printf("cridem a getkey pare\n");
  		int ret = getkey(b, time);
  		if(ret != -1) printf("Fet:  %s\n", b);
  		else printf("S'ha acabat el temps pare\n");
  	}
}

void test_screen() {
	Word screen[80][25];
	for (int i = 0; i < 80; i++)
		for (int j = 0; j < 25; j++) {
			screen[i][j] = '0' + (2 << 8);
		}

	clrscr(screen);
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
	
	test_screen();
  while(1){
  	
  }
}
