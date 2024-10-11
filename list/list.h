#ifndef LIST_H
#define LIST_H


/**
 * Circular Doubly-Linked List
 * - `next`: pointer to the next element, or to itself if the list is empty.
 * - `prev`: pointer to the previous element, or to itself if the list is empty.
 */
struct head {
  struct head *prev, *next;
};
typedef struct head head;

void init(head * h);

void add(head * new, head * cur);
void del(head * cur);

#define entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define foreach(head, it) \
  for (it = head->next; it != head; it = it->next)

#endif