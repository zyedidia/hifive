#include "libfive/uart.h"
#include "libfive/gpio.h"
#include "libfive/timer.h"

int main() {
    unsigned pin = 20;
    gpio_set_input(pin);

    while (1) {
        printf("pin: %d\n", gpio_read(pin));
        delay_ms(100);
    }
}
