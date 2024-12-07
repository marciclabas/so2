/*
 * sync.h - Estructures i macros per la sincronització de threads
 */
#include <list.h>
#include <types.h>
#include <mm_address.h>

#define NR_SEM 10

typedef struct sem_t{
	list_head list;
	int TID_owner;
	int count;
 	list_head blocked;
 	int id; // no es quedara aixi pero de moment
} sem_t;


void init_semafors();
int sys_semCreate(int init_value);
int sys_semWait(int id);
int sys_semSignal(int id);
int sys_semDestroy(int id);
