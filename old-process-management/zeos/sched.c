/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>
#include <system.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

struct list_head freequeue;
struct list_head readyqueue;

struct task_struct * idle_task;

int pid_counter = 2;
int new_pid() {
	return pid_counter++;
}

struct task_struct *list_head_to_task_struct(struct list_head *l) {
  return list_entry( l, struct task_struct, list);
}

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void) {
	__asm__ __volatile__("sti": : :"memory");
	while(1);
}

void init_idle() {
	struct list_head * head = list_first(&freequeue);
	list_del(head);
	union task_union * task = list_head_to_task_struct(head);
	
	task->task.PID = 0;
	allocate_DIR(&task->task);

	// prepare for context switch:
	// - stack: [@ret, ebp] (ebp can be whatever since `cpu_idle` doesn't use the stack nor will ever return)
	// - task.kernel_esp <- stack at ebp
	task->stack[KERNEL_STACK_SIZE-2] = 0;
	task->stack[KERNEL_STACK_SIZE-1] = &cpu_idle;
	task->task.kernel_esp = &task->stack[KERNEL_STACK_SIZE-2];

	idle_task = &task->task;
}

void init_task1() {
	struct list_head * head = list_first(&freequeue);
	list_del(head);
	union task_union * task = list_head_to_task_struct(head);
	
	set_quantum(&task->task, 1000);
	task->task.PID = 1;
	allocate_DIR(&task->task);
	set_user_pages(&task->task);
	task->task.kernel_esp = &task->stack[KERNEL_STACK_SIZE];
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
}

void inner_task_switch(union task_union * new) {
	current()->kernel_esp = read_ebp();
	set_cr3(new->task.dir_pages_baseAddr);
	tss.esp0 = new->task.kernel_esp;
	write_msr(0x175, new->task.kernel_esp);
	ret_task_switch(new->task.kernel_esp);
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

int get_quantum(struct task_struct * t) {
	return t->quantum;
}

void set_quantum(struct task_struct * t, int quantum) {
	t->quantum = quantum;
}

int remaining_ticks = 0;
void update_sched_data_rr() {
	remaining_ticks--;
}

int needs_sched_rr() {
	return remaining_ticks < 0 && !list_empty(&readyqueue);
}

// void update_process_state_rr(struct task_struct *t, struct list_head *dst) {
// 	if (dst == NULL) {
		
// 	}
// }

void sched_next_rr() {
	struct task_struct * curr = current();
	if (curr != idle_task) {
		list_add_tail(&curr->list, &readyqueue);
	}
	struct list_head * next = list_first(&readyqueue);
	list_del(next);
	struct task_struct * next_task = list_head_to_task_struct(next);
	remaining_ticks = get_quantum(next_task);
	task_switch(next_task);
}

void schedule() {
	update_sched_data_rr();
	int reschedule = needs_sched_rr();
  if (reschedule)
    sched_next_rr();
}

void print_pcb(struct task_struct *pcb) {
	printf("PID: %d\n", pcb->PID);
	printf("kernel_esp: %d\n", pcb->kernel_esp);
	printf("quantum: %d\n", pcb->quantum);
	printf("state: %d\n", pcb->state);
}