OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
    /* QEMU virt + OpenSBI normally enters the S-mode payload at 0x80200000. */
    . = 0x80200000;

    .text : ALIGN(4096) {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }
    .rodata : ALIGN(4096) { *(.rodata .rodata.*) }
    .data : ALIGN(4096) { *(.data .data.*) *(.sdata .sdata.*) }
    .bss : ALIGN(4096) {
        __bss_start = .;
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    }

    /DISCARD/ : { *(.comment) *(.eh_frame*) *(.note*) }
}
