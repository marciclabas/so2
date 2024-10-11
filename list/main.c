#include "list.h"
#include <stdio.h>

typedef struct elem {
  int value;
  head anchor;
} elem;

int main() {
  
  elem list[10];
  elem* h = &list[0];
  head* anchor = &h->anchor;
  h->value = 0;
  
  init(&h->anchor);
  for (int i = 1; i < 10; i++) {
    add(&list[i-1].anchor, &list[i].anchor);
    list[i].value = i;
  }

  head* it;
  foreach(anchor, it) {
    elem* e = entry(it, elem, anchor);
    printf("%d\n", e->value);
  }
}