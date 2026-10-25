    .section .text.boot, "ax"
    .globl _start
    .align 2
_start:
    /* This starter runs exactly one QEMU hart, so a single stack is safe. */
    /* Keep a0=hart ID and a1=device-tree address for kernel_main. */
    la t0, __bss_start
    la t1, __bss_end
1:
    bgeu t0, t1, 2f
    sd zero, 0(t0)
    addi t0, t0, 8
    j 1b
2:
    la sp, boot_stack_top
    call kernel_main
3:
    wfi
    j 3b

    .section .bss.stack, "aw", @nobits
    .align 12
boot_stack_bottom:
    .skip 16384
    .globl boot_stack_top
boot_stack_top:
