#include "page_pool.h"
#include "sbi.h"
#include "uart.h"

#include <stdint.h>

static int kernel_self_test(void)
{
    void *first = page_pool_alloc();
    void *second = page_pool_alloc();
    if (first == 0 || second == 0 || first == second) {
        return -1;
    }
    if (((uintptr_t)first % PAGE_SIZE) != 0 ||
        ((uintptr_t)second % PAGE_SIZE) != 0) {
        return -1;
    }
    if (page_pool_free(first) != 0 || page_pool_free(first) != -1) {
        return -1;
    }
    if (page_pool_free(second) != 0) {
        return -1;
    }
    return page_pool_free_count() == PAGE_POOL_COUNT ? 0 : -1;
}

void kernel_main(uintptr_t hartid, uintptr_t dtb)
{
    uart_puts("[starter] RISC-V64 supervisor payload entered\n");
    uart_puts("[starter] hart = ");
    uart_puthex(hartid);
    uart_puts("  dtb = ");
    uart_puthex(dtb);
    uart_puts("\n");

    page_pool_init();
    if (kernel_self_test() == 0) {
        uart_puts("BOOT_OK: UART and page pool self-test passed\n");
    } else {
        uart_puts("BOOT_FAIL: page pool self-test failed\n");
    }
    sbi_shutdown();
}
