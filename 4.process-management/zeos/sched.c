/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>


union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

list_head freequeue[NR_TASKS];
list_head readyqueue[NR_TASKS];

task_struct *list_head_to_task_struct(list_head *l) {
  return list_entry( l, task_struct, list);
}

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(task_struct *t)  {
	int pos = ((int)t-(int)task)/sizeof(union task_union);
	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 
	return 1;
}

void cpu_idle(void) {
	__asm__ __volatile__("sti": : :"memory");
	while(1) {}
}

void init_idle(void) {
	union task_union * t = list_head_to_task_struct(&freequeue);
	task_struct * pcb = &t->task;
	unsigned long * stack = t->stack;
	
	list_del(&freequeue);
	pcb->PID = 0;
	allocate_DIR(pcb);
	
	stack[KERNEL_STACK_SIZE-1] = (unsigned long) &cpu_idle;
	stack[KERNEL_STACK_SIZE-2] = 0x0;
	pcb->kernel_esp = (unsigned long) &stack[KERNEL_STACK_SIZE-2];
}

void init_task1(void)
{
}


void init_sched() {
	INIT_LIST_HEAD(&freequeue);
	for (int i = 0; i < NR_TASKS; i++)
		list_add_tail(&task[i].task.list, &freequeue);

	INIT_LIST_HEAD(&readyqueue);
}

task_struct* current() {
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (task_struct*)(ret_value&0xfffff000);
}

