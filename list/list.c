#include "list.h"

// Empty List: prev and next point to itself
void init(head* h) {
  h->next = h;
  h->prev = h;
}

/* Add `new` after `cur`
 * - From: `prev -> cur -> next`
 * - To:   `prev -> cur -> new -> next`
 */
void add(head* cur, head* new) {
  head* prev = cur->prev;
  head* next = cur->next;

  prev->next = cur;
  cur->prev = prev;
  cur->next = new;

  new->prev = cur;
  new->next = next;
  next->prev = new;
}

/* Delete `cur`
 * - From: `prev -> cur -> next`
 * - To:   `prev -> next`
 * - `cur` is left in an undefined state (nulls)
 */
void del(head* cur) {
  head* prev = cur->prev;
  head* next = cur->next;

  prev->next = next;
  next->prev = prev;

  cur->next = (void*) 0;
  cur->prev = (void*) 0;
}