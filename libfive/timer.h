#pragma once

#include "mem.h"

static unsigned read_ticks() {
    return get32((const volatile void*) 0x200BFF8);
}

static unsigned read_us(void) {
    unsigned u = read_ticks();
    return u * 31;
}

static unsigned read_ms(void) {
    unsigned u = read_ticks();
    return u / 32;
}

static unsigned read_s(void) {
    unsigned u = read_ticks();
    return u / 32768;
}

static void delay_us(unsigned us) {
    unsigned rb = read_us();
    while (1) {
        unsigned ra = read_us();
        if ((ra - rb) >= us) {
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
