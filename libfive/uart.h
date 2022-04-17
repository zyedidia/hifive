#pragma once

#include "libc/tinyprintf.h"

void uart_init(unsigned baud);
int uart_can_putc();
void uart_putc(void* p, char c);
