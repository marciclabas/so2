#ifndef __SLAB_H__
#define __SLAB_H__

/**
 * Linked list of objects of a fixed size.
 * 
 * - The first sizeof(void*) bytes of each section contain a pointer to the next free section (or NULL)
 * - `free` points to head of the list of free sections (or NULL if none)
 */
typedef struct slab_t {
  void* data;
  void* free;
} slab_t;

/** Allocate a new section for slabs of `max(obj_size, sizeof(void*))`. Returns NULL if unable to allocate. */
slab_t* slab_create(int obj_size, int pages);

/** Free the memory used by the slab. */
void slab_destroy(slab_t* slab);

/** Allocate a section from the slab. Returns NULL if none available. */
void* slab_alloc(slab_t* slab);

/** Free the `obj` allocation in the slab */
void slab_free(slab_t* slab, void* obj);

void test_slab();

#endif