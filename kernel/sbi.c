#include "sbi.h"

/* SBI system-reset extension (SRST), function 0. Runs under OpenSBI. */
void sbi_shutdown(void)
{
    register long a0 __asm__("a0") = 0; /* reset_type=0: shutdown */
    register long a1 __asm__("a1") = 0; /* reset_reason=0: no reason */
    register long a6 __asm__("a6") = 0;
    register long a7 __asm__("a7") = 0x53525354;
    __asm__ volatile("ecall"
                     : "+r"(a0), "+r"(a1)
                     : "r"(a6), "r"(a7)
                     : "memory");
    for (;;) {
        __asm__ volatile("wfi");
    }
}
