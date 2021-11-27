#pragma once

#include "libc/tinyprintf.h"

int uart_can_putc();
void uart_putc(void* p, char c);
