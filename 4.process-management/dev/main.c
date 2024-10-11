#include <stdio.h>
#include "list.h"

typedef struct {
  int pid;
  list_head list;
} task_struct;

int main() {
  task_struct task[10];
  list_head freequeue;
  INIT_LIST_HEAD(&freequeue);

  for (int i = 0; i < 10; i++) {
    task[i].pid = i;
    list_add(&task[i].list, &freequeue);
  }

  list_head * it;
  list_for_each(it, &freequeue) {
    task_struct * t = list_entry(it, task_struct, list);
    printf("Task %d\n", t->pid);
  }
}