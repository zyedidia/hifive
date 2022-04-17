#include "libfive/uart.h"
#include "libfive/exception.h"
#include "libfive/gpio.h"
#include "libfive/riscv_csr.h"

#define PIN 20

void __attribute__((interrupt, aligned(16))) irq_handler() {
    if (gpio_irq_fall(PIN)) {
        printf("fall\n");
    } else if (gpio_irq_rise(PIN)) {
        printf("rise\n");
    }
    gpio_irq_clear(PIN);
    plic_claim_clear();
}

int main() {
    unsigned pin = PIN; // pin marked '4'

    plic_init();
    plic_enable(gpio_irq_num(pin));
    plic_set_priority(gpio_irq_num(pin), 7);

    exception_init(irq_handler);
    gpio_set_input(pin);
    gpio_enable_irq(pin);

    while (1) {}

    return 0;
}
