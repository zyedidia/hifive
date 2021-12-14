#include "uart.h"
#include "gpio.h"
#include "timer.h"

extern int main();

extern int __data_start__, __data_start_flash__, __data_end__, __bss_start__,
    __bss_end__;

void _cstart() {
    int* data = &__data_start__;
    int* data_end = &__data_end__;
    int* data_flash = &__data_start_flash__;

    while (data < data_end) {
        *data++ = *data_flash++;
    }

    int* bss = &__bss_start__;
    int* bss_end = &__bss_end__;

    while (bss < bss_end) {
        *bss++ = 0;
    }

#ifdef RVSYM
    gpio_init();
#endif

    init_printf(NULL, uart_putc);

    main();

#ifdef RVSYM
    rvsym_quiet_exit();
#endif
}
