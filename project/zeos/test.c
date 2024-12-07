#include <libc.h>

sem_t* s;
sem_t* s2;

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

void sem1(void* arg){
  printf("...thread2 intento entrar a la zona bloquejada\n");
  semWait(s);
  printf("...thread2 per fi he entrat...\n");
  sleep(1000);
  semSignal(s);
  printf("...thread2 surto de la zona protegida\n");
  printf("...thread2 intento borrar el sem pero no podre...\n");
  semDestroy(s);
  printf("...thread2 desbloquejo al primer thread del sem de sincronització\n");
  semSignal(s2);
  sleep(1000);
  printf("...thread2 exit!");
  exit();
}


  
void test_sem(){

  s = semCreate(1);
  printf("Comença el test!!!!!!\n");
  s2 = semCreate(0);
  printf("semàfor creat!\n");
  threadCreateWithStack(sem1, 1, (void*) 1);
  semWait(s);
  printf("thread1 entro a la zona protegida i sleep...\n");
  sleep(1000);
  printf("thread1 ja he dormit prou\n");
  semSignal(s);
  printf("thread1 ara intento passar el semafor de sincronització\n");
  semWait(s2);
  printf("thread1 ja he passat el semafor de sincronització\n");
  printf("thread1 elimino els semafors\n");
  semDestroy(s);
  semDestroy(s2);
  printf("thread1 he acabat\n");
}
