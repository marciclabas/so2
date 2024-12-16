#include <sync.h>
#include <mm.h>
#include <io.h>
#include <system.h>

struct sem_t all_sem[NR_SEM];
	__attribute__((__section__(".data.task")));
struct list_head free_sem;
struct list_head ready_sem;

sem_t *list_head_to_sem(list_head *l) {
	return list_entry(l, sem_t, list);
}

int count_id = 0;
int new_id(){
	return count_id++;
}
void init_semafors() {
	// freesems <- all sems
	INIT_LIST_HEAD(&free_sem);
	
	for (int i = 0; i < NR_SEM; i++){
		list_add_tail(&all_sem[i].list, &free_sem);
		all_sem[i].active = 0;
	}
	INIT_LIST_HEAD(&ready_sem);
}


int sys_semCreate(int init_value){
	if(!list_empty(&free_sem)){
		list_head* h = list_pop(&free_sem);
		sem_t* s = list_head_to_sem(h);
		list_add_tail(&s->list,&ready_sem);
		list_add_tail(&s->sem_anchor, &current()->sems_created);
		s->TID_owner = current()->TID;
		s->count = init_value;
		s->id = new_id();
		s->active = 1;
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
	if(s == 0 || s->active == 0) return -1;
	s->count--;
	if(s->count < 0){
		list_add(&current()->list, &s->blocked);
		sched_next_rr();
	}
	if(current()->sem_destroyed == 1){
		current()->sem_destroyed = 0;
		return -1;
	}
	return 1;
}

int sys_semSignal(int id){
	sem_t* s = getSem(id);
	s->count++;
	//printf("sys_semSignal");
	if(s->count <= 0 && !list_empty(&s->blocked)){
		//printf("desbloquejo proces\n");
		list_head* t = list_pop(&s->blocked);
		list_add_tail(t, &readyqueue);
		task_struct* p = list_head_to_task_struct(t);
		p->sem_destroyed = 0;
	}
	//printf("...\n");
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
		task_struct* p = list_head_to_task_struct(t);
		p->sem_destroyed = 1;
		list_del(&s->sem_anchor);
	}
	
	list_del(&s->list);
	list_add_tail(&s->list, &free_sem);
	return 1;
}

int sem_destroy(list_head* h){
	sem_t* s = list_head_to_sem(h);
	return sys_semDestroy(s->id);
}









