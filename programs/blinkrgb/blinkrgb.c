#include "libfive/gpio.h"
#include "libfive/timer.h"

#define LED_RED 22
#define LED_GREEN 19
#define LED_BLUE 21

void setup() {
    gpio_set_output(LED_RED);
    gpio_set_output(LED_GREEN);
    gpio_set_output(LED_BLUE);
    gpio_set_xor(LED_RED, 1);
    gpio_set_xor(LED_GREEN, 1);
    gpio_set_xor(LED_BLUE, 1);
}

void set_rgb(unsigned r, unsigned g, unsigned b) {
    gpio_write(LED_RED, r);
    gpio_write(LED_GREEN, g);
    gpio_write(LED_BLUE, b);
}

int main() {
    setup();

    while (1) {
        set_rgb(1, 0, 0);
        delay_ms(500);
        set_rgb(0, 1, 0);
        delay_ms(500);
        set_rgb(0, 0, 1);
        delay_ms(500);
    }

    return 0;
}
