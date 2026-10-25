#include "uart.h"

/* QEMU 'virt' uses a 16550-compatible UART at this address. */
#define UART0_BASE ((uintptr_t)0x10000000u)
#define UART_THR 0u
#define UART_LSR 5u
#define UART_LSR_THR_EMPTY 0x20u

static volatile uint8_t *uart_reg(unsigned offset)
{
    return (volatile uint8_t *)(UART0_BASE + (uintptr_t)offset);
}

void uart_putc(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }
    while ((*uart_reg(UART_LSR) & UART_LSR_THR_EMPTY) == 0) {
        /* Poll until transmitter can accept another byte. */
    }
    *uart_reg(UART_THR) = (uint8_t)c;
}

void uart_puts(const char *s)
{
    while (*s != '\0') {
        uart_putc(*s++);
    }
}

void uart_puthex(uintptr_t value)
{
    static const char digits[] = "0123456789abcdef";
    uart_puts("0x");
    for (int shift = (int)(sizeof(uintptr_t) * 8u) - 4; shift >= 0; shift -= 4) {
        uart_putc(digits[(value >> (unsigned)shift) & 0xfu]);
    }
}
