#include "uart.h"
#include "bits.h"

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

typedef enum {
    TXDATA_FULL = 31,
} uart_txdata_t;

static volatile uart_reg_t* const uart_0 = (uart_reg_t*) 0x10013000;

int uart_can_putc() {
    return bit_get(uart_0->txdata, TXDATA_FULL) == 0;
}

// p needed for compatibility with tinyprintf
void uart_putc(void* p, char c) {
    (void) p; // ignore
    while (!uart_can_putc())
        ;
    uart_0->txdata = c;
}
