#ifndef STARTER_PAGE_POOL_H
#define STARTER_PAGE_POOL_H

#include <stddef.h>

/* Temporary private memory pool; NOT a RAM-discovering physical allocator. */
#define PAGE_SIZE 4096u
#define PAGE_POOL_COUNT 16u

void page_pool_init(void);
void *page_pool_alloc(void);
int page_pool_free(void *address);
size_t page_pool_free_count(void);

#endif
