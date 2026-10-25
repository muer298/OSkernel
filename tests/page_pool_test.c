#include "page_pool.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    void *pages[PAGE_POOL_COUNT];
    page_pool_init();
    assert(page_pool_free_count() == PAGE_POOL_COUNT);

    for (size_t i = 0; i < PAGE_POOL_COUNT; ++i) {
        pages[i] = page_pool_alloc();
        assert(pages[i] != NULL);
        assert(((uintptr_t)pages[i] % PAGE_SIZE) == 0);
        assert(((unsigned char *)pages[i])[0] == 0);
        ((unsigned char *)pages[i])[0] = 0x5a;
        for (size_t j = 0; j < i; ++j) {
            assert(pages[i] != pages[j]);
        }
    }
    assert(page_pool_alloc() == NULL);
    assert(page_pool_free_count() == 0);
    assert(page_pool_free(&pages[0]) == -1);
    assert(page_pool_free(pages[0]) == 0);
    assert(page_pool_free(pages[0]) == -1);
    void *reallocated = page_pool_alloc();
    assert(reallocated == pages[0]);
    assert(((unsigned char *)reallocated)[0] == 0);
    assert(page_pool_free(reallocated) == 0);
    for (size_t i = 1; i < PAGE_POOL_COUNT; ++i) {
        assert(page_pool_free(pages[i]) == 0);
    }
    assert(page_pool_free_count() == PAGE_POOL_COUNT);
    puts("PASS: 16-page pool allocation, exhaustion, alignment, free, double-free");
    return 0;
}
