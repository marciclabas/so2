#include <sync.h>
#include <mm.h>
#include <io.h>
#include <system.h>

struct sem_t all_sem[NR_SEM];
	__attribute__((__section__(".data.task")));
struct list_head free_sem;
struct list_head ready_sem;

task_struct *list_head_to_sem(list_head *l) {
	return list_entry(l, sem_t, list);
}

void init_semafors() {
	// freesems <- all sems
	INIT_LIST_HEAD(&free_sem);
	
	for (int i = 0; i < NR_SEM; i++){
		list_add_tail(&all_sem[i].list, &free_sem);
		all_sem[i].id = i;
	}
	INIT_LIST_HEAD(&ready_sem);
}


int sys_semCreate(int init_value){
	if(!list_empty(&free_sem)){
		list_head* h = list_pop(&free_sem);
		sem_t* s = list_head_to_sem(h);
		list_add_tail(&s->list,&ready_sem);
		s->TID_owner = current()->TID;
		s->count = init_value;
		INIT_LIST_HEAD(&s->blocked);
		return s->id;
	}
	return -1;
}

sem_t* getSem(int id){
	list_head *it;
	list_for_each(it, &ready_sem){
		sem_t *s = list_head_to_sem(it);
		if(s->id == id) return s;
	}
	return 0;
}


int sys_semWait(int id){
	sem_t* s = getSem(id);
	if(s == 0) return -1;
	s->count--;
	if(s->count < 0){
		list_add(&current()->list, &s->blocked);
		sched_next_rr();
	}
	return 1;
}

int sys_semSignal(int id){
	sem_t* s = getSem(id);
	s->count++;
	
	if(s->count <= 0 && !list_empty(&s->blocked)){
		//printf("desbloquejo proces\n");
		list_head* t = list_pop(&s->blocked);
		list_add_tail(t, &readyqueue);
	}
}

int sys_semDestroy(int id){
	sem_t* s = getSem(id);
	if(current()->TID != s->TID_owner) {
		//printf("No he eliminat el semafor\n");
		return -1;
	}
	while(!list_empty(&s->blocked)){ //----------------------------> No se si volem ferho aixi o com
		list_head* t = list_pop(&s->blocked);
		list_add_tail(t, &readyqueue);
	}
	
	list_del(&s->list);
	list_add_tail(&s->list, &free_sem);
	return 1;
}










