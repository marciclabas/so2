#include <libc.h>
#include <slab.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

slab_t* slab_create(int obj_size, int pages) {
  slab_t* slab = (slab_t*) memRegGet(pages);
  if (!slab)
    return 0;

  unsigned long offset = max(sizeof(slab_t), obj_size); // align to obj_size
  int objects = (4096*pages - offset) / obj_size;

  slab->data = slab->free = (void*) slab + offset;

  // data[i] = @data[i+1]
  // data[n-1] = 0
  void* curr = slab->data;
  for (int i = 0; i < objects; i++) {
    void** next = i < objects - 1
      ? curr + obj_size
      : 0;
    *(void**) curr = next;
    curr = next;
  }

  return slab;
}

void slab_destroy(slab_t* slab) {
  memRegDel((char*) slab);
}

void* slab_alloc(slab_t* slab) {
  void* obj = slab->free;
  if (obj)
    slab->free = *(void **) obj;
  return obj;
}

void slab_free(slab_t* slab, void* obj) {
  void* next = slab->free;
  *(void **) obj = next;
  slab->free = obj;
}








void test_slab() {
  slab_t* slab = slab_create(16, 1);
  if (!slab) {
    printf("Error: slab_create failed\n");
    return;
  }
  else
    printf("slab_create succeeded\n");

  void* objs[10];
  for (int i = 0; i < 10; i++) {
    objs[i] = slab_alloc(slab);
    if (!objs[i]) {
      printf("Error: slab_alloc failed\n");
      return;
    }
  }
  printf("slab_alloc succeeded\n");

  for (int i = 0; i < 10; i++) {
    slab_free(slab, objs[i]);
  }
  printf("slab_free succeeded\n");

  for (int i = 0; i < 10; i++) {
    objs[i] = slab_alloc(slab);
    if (!objs[i]) {
      printf("Error: slab_alloc (anew) failed\n");
      return;
    }
  }
  printf("slab_alloc (anew) succeeded\n");

  slab_destroy(slab);
}