#ifndef _KOZOS_MEMORY_H_INCLUDED_
#define _KOZOS_MEMORY_H_INCLUDED_

int kmem_init(void);
void *kzmem_alloc(int size);
void kzmem_free(void *mem);

#endif
