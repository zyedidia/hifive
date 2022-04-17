#include "libfive/uart.h"
#include "libfive/exception.h"
#include "libfive/gpio.h"
#include "libfive/riscv_csr.h"

void __attribute__((interrupt, aligned(16))) irq_handler() {
    printf("interrupt %ld %d %d\n", read_csr(mcause) & 0xffff, gpio_irq_fall(23), gpio_irq_rise(23));
    gpio_irq_clear(23);
    printf("%d\n", gpio_irq_rise(23));
    plic_claim_clear();
    printf("%d\n", gpio_irq_rise(23));
}

int main() {
    unsigned pin = 23; // pin marked '7'

    plic_init();
    plic_enable(gpio_irq_num(pin));
    plic_set_priority(gpio_irq_num(pin), 7);

    exception_init(irq_handler);
    gpio_set_input(pin);
    gpio_enable_irq(pin);

    while (1) {}

    return 0;
}
