#pragma once

#include "cpu.h"
#include "mem.h"

static unsigned read_ticks() {
    return get32((const volatile void*) 0x200BFF8);
}

static void delay_ticks(unsigned ticks) {
    unsigned rb = read_ticks();
    while (1) {
        unsigned ra = read_ticks();
        if ((ra - rb) >= ticks) {
            break;
        }
    }
}

static void delay_us(unsigned us) {
    unsigned rb = read_ticks();
    while (1) {
        unsigned ra = read_ticks();
        if (((ra - rb) * CPU_FREQ / (1000 * 1000)) >= us) {
            break;
        }
    }
}

static void delay_ms(unsigned ms) {
    delay_us(ms * 1000);
}

static void delay_sec(unsigned sec) {
    delay_ms(sec * 1000);
}
