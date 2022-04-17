#include "libfive/gpio.h"
#include "libfive/timer.h"

// connect to the pin marked '4'
#define LED_PIN 20

int main() {
    gpio_set_output(LED_PIN);

    unsigned val = 0;

    while (1) {
        gpio_write(LED_PIN, val);
        val = !val;

        delay_ms(500);
    }
}
