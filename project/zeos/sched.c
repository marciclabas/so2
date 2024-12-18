/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <system.h>

task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

struct list_head freequeue;
struct list_head readyqueue;

task_struct * idle_task;

int pid_counter = 2;
int new_pid() {
	return pid_counter++;
}

int tid_counter = 2;
int new_tid() {
	return tid_counter++;
}

task_struct *list_head_to_task_struct(list_head *l) {
  return list_entry(l, task_struct, list);
}

task_struct *list_head_to_child(list_head *l) {
	return list_entry(l, task_struct, child_anchor);
}

task_struct *list_head_to_thread(list_head *l) {
	return list_entry(l, task_struct, thread_anchor);
}

extern list_head blocked;


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


int allocate_DIR(task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void) {
	printf("cpu_idle\n");
	__asm__ __volatile__("sti": : :"memory");
	while(1);
}

void init_pcb(task_struct * t) {
	t->PID = -1;
	t->TID = -1;
	t->quantum = 0;
	t->state = ST_RUN;
	t->pending_unblocks = 0;
	t->thread_principal = 1;
	INIT_LIST_HEAD(&t->children);
	INIT_LIST_HEAD(&t->threads_created);
	INIT_LIST_HEAD(&t->sems_created);
}

void init_idle() {
	list_head * head = list_pop(&freequeue);
	task_union * task = (task_union*) list_head_to_task_struct(head);
	
	task->task.PID = 0;
	task->task.TID = 0;
	allocate_DIR(&task->task);

	// prepare for context switch:
	// - stack: [@ret, ebp] (ebp can be whatever since `cpu_idle` doesn't use the stack nor will ever return)
	// - task.kernel_esp <- stack at ebp
	task->stack[KERNEL_STACK_SIZE-2] = 0;
	task->stack[KERNEL_STACK_SIZE-1] = (unsigned long) &cpu_idle;
	task->task.kernel_esp = (unsigned int) &task->stack[KERNEL_STACK_SIZE-2];
	
	idle_task = &task->task;
	init_pcb(idle_task);
}

void init_task1() {
	list_head * head = list_pop(&freequeue);
	task_union * task = (task_union*) list_head_to_task_struct(head);

	init_pcb(&task->task);
	list_add_tail(&task->task.child_anchor, &idle_task->children);
	
	set_quantum(&task->task, 100);
	task->task.PID = 1;
	task->task.TID = 1;
	allocate_DIR(&task->task);
	set_user_pages(&task->task);
	task->task.kernel_esp = (unsigned int) &task->stack[KERNEL_STACK_SIZE];
	write_msr(0x175, task->task.kernel_esp);
	tss.esp0 = task->task.kernel_esp;
	set_cr3(task->task.dir_pages_baseAddr);
}


void init_sched() {
	// freequeue <- all tasks
	INIT_LIST_HEAD(&freequeue);
	for (int i = 0; i < NR_TASKS; i++)
		list_add_tail(&(task[i].task.list), &freequeue);

	// readyqueue <- empty
	INIT_LIST_HEAD(&readyqueue);
	INIT_LIST_HEAD(&blocked);
}

void inner_task_switch(task_union * new) {
	current()->kernel_esp = read_ebp();
	tss.esp0=(int)&(new->stack[KERNEL_STACK_SIZE]);
  write_msr(0x175, (unsigned long)&(new->stack[KERNEL_STACK_SIZE]));
	set_cr3(new->task.dir_pages_baseAddr);
	ret_task_switch(new->task.kernel_esp);
}

task_struct* current() {
  int ret_value;
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (task_struct*)(ret_value&0xfffff000);
}

int get_quantum(task_struct * t) {
	return t->quantum;
}

void set_quantum(task_struct * t, int quantum) {
	t->quantum = quantum;
}

int remaining_ticks = 0;
void update_sched_data_rr() {
	remaining_ticks--;
}

int needs_sched_rr() {
	return remaining_ticks < 0 && !list_empty(&readyqueue);
}

void update_process_state_rr(task_struct *t, struct list_head *dst) {
	if (t != idle_task)
		list_add_tail(&t->list, dst);
}

void sched_next_rr() {
	if (list_empty(&readyqueue)) {
		task_switch((task_union*) idle_task);
	}
	else {
		printf("(");
		printf("%d , ",current()->TID);
		list_head * next = list_pop(&readyqueue);
		task_struct * next_task = list_head_to_task_struct(next);
		remaining_ticks = get_quantum(next_task);
		task_switch((task_union*) next_task);
		printf("%d",current()->TID);
		printf(")");
	}
}

void schedule() {
	update_sched_data_rr();
  if (needs_sched_rr()) {
		update_process_state_rr(current(), &readyqueue);
    sched_next_rr();
	}
}

void print_pcb(task_struct *pcb) {
	printf("PID: %d\n", pcb->PID);
	printf("kernel_esp: %d\n", pcb->kernel_esp);
	printf("quantum: %d\n", pcb->quantum);
	printf("state: %d\n", pcb->state);
}
