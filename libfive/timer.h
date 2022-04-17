#pragma once

#include "cpu.h"
#include "mem.h"
#include "riscv_csr.h"

static unsigned read_cycles() {
    return read_csr(mcycle);
}

static void delay_ticks(unsigned ticks) {
    unsigned rb = read_cycles();
    while (1) {
        unsigned ra = read_cycles();
        if ((ra - rb) >= ticks) {
            break;
        }
    }
}

static void delay_us(unsigned us) {
    unsigned rb = read_cycles();
    while (1) {
        unsigned ra = read_cycles();
        if ((ra - rb) >= us * (CPU_FREQ / (1000 * 1000))) {
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
