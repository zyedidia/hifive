#include "uart.h"

#include "libc/tinyprintf.h"

typedef struct {
    unsigned txdata;
    unsigned rxdata;
    unsigned txctrl;
    unsigned rxctrl;
    unsigned ie;
    unsigned ip;
    unsigned div;
} uart_reg_t;

static volatile uart_reg_t* const uart_0 = (uart_reg_t*) 0x10013000;
static volatile uart_reg_t* const uart_1 = (uart_reg_t*) 0x10023000;

int uart_can_putc() {
    return uart_0->txdata == 0;
}

// p needed for compatibility with tinyprintf
void uart_putc(void* p, char c) {
    (void) p;
    while (!uart_can_putc())
        ;
    uart_0->txdata = c;
}
