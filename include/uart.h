#ifndef STARTER_UART_H
#define STARTER_UART_H

#include <stdint.h>

void uart_putc(char c);
void uart_puts(const char *s);
void uart_puthex(uintptr_t x);

#endif
