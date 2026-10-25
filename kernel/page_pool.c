#include "page_pool.h"

#include <stdint.h>

/* The pool lives within the kernel's own BSS to avoid firmware/FDT overlap. */
static uint8_t pool[PAGE_POOL_COUNT][PAGE_SIZE] __attribute__((aligned(PAGE_SIZE)));
static uint8_t used[PAGE_POOL_COUNT];

void page_pool_init(void)
{
    for (size_t i = 0; i < PAGE_POOL_COUNT; i++) {
        used[i] = 0;
    }
}

void *page_pool_alloc(void)
{
    for (size_t i = 0; i < PAGE_POOL_COUNT; i++) {
        if (!used[i]) {
            used[i] = 1;
            for (size_t j = 0; j < PAGE_SIZE; j++) {
                pool[i][j] = 0;
            }
            return pool[i];
        }
    }
    return NULL;
}

int page_pool_free(void *address)
{
    for (size_t i = 0; i < PAGE_POOL_COUNT; i++) {
        if (address == (void *)pool[i]) {
            if (!used[i]) {
                return -1; /* Double-free is rejected. */
            }
            used[i] = 0;
            return 0;
        }
    }
    return -1; /* Reject arbitrary/unowned pointers. */
}

size_t page_pool_free_count(void)
{
    size_t count = 0;
    for (size_t i = 0; i < PAGE_POOL_COUNT; i++) {
        count += !used[i];
    }
    return count;
}
