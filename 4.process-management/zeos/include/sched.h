/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include <list.h>
#include <types.h>
#include <mm_address.h>

#define NR_TASKS      10
#define KERNEL_STACK_SIZE	1024

typedef enum state_t { ST_RUN, ST_READY, ST_BLOCKED } state_t;

typedef struct task_struct {
  int PID;			/* Process ID. This MUST be the first field of the struct. */
  page_table_entry * dir_pages_baseAddr;
  unsigned int kernel_esp;
  list_head list;
} task_struct;

typedef union task_union {
  task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
} task_union;

extern task_union task[NR_TASKS]; /* Vector de tasques */

#define KERNEL_ESP(t)       	(DWord) &(t)->stack[KERNEL_STACK_SIZE]

#define INITIAL_ESP       	KERNEL_ESP(&task[1])

void init_task1(void);
void init_idle(void);
void init_sched(void);

task_struct * current();

void task_switch(task_union*t);

task_struct *list_head_to_task_struct(list_head *l);

int allocate_DIR(task_struct *t);

page_table_entry * get_PT (task_struct *t) ;

page_table_entry * get_DIR (task_struct *t) ;

/* Headers for the scheduling policy */
void sched_next_rr();
void update_process_state_rr(task_struct *t, list_head *dest);
int needs_sched_rr();
void update_sched_data_rr();

#endif  /* __SCHED_H__ */
