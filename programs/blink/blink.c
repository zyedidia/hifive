#include "libfive/gpio.h"
#include "libfive/timer.h"

#ifdef RVSYM
#include "rvsym.h"
#endif

int main() {
    unsigned pin = 23; // pin marked 7 on board

#ifdef RVSYM
    rvsym_mark_bytes(&pin, sizeof(pin), "pin");
#endif

    gpio_set_output(pin);

    unsigned val = 0;

    for (int i = 0; i < 10; i++) {
        gpio_write(pin, val);
        val = !val;

        delay_ms(500);
    }

    return 0;
}
