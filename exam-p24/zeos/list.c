#include <list.h>

/* 
 * Initializes an empty list.
 */
void INIT_LIST_HEAD(list_head *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(list_head *new,
			      list_head *prev,
			      list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
void list_add(list_head *new, list_head *head)
{
	__list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void list_add_tail(list_head *new, list_head *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(list_head * prev, list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
void list_del(list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void*) 0;
	entry->prev = (void*) 0;
}

list_head *list_pop(list_head *head) {
	list_head *first = head->next;
	list_del(first);
	return first;
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
int list_is_last(const list_head *list,
				const list_head *head)
{
	return list->next == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
int list_empty(const list_head *head) {
	return head->next == head;
}

int list_len(const list_head *head) {
	int len = 0;
	list_head *it = head->next;
	list_for_each(it, head) {
		len++;
	}
	return len;
}

list_head* list_find(int (*pred)(list_head*), list_head *head) {
	list_head *it = head->next;
	list_for_each(it, head) {
		if (pred(it))
			return it;
	}
	return 0;
}
