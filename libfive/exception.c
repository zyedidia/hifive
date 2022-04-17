#include "riscv_csr.h"
#include "uart.h"
#include "bits.h"

static volatile unsigned* const plic_priority = (unsigned*) 0x0C000004;
static volatile unsigned* const plic_pending = (unsigned*) 0x0C001000;
static volatile unsigned* const plic_enables = (unsigned*) 0x0C002000;
static volatile unsigned* const plic_threshold = (unsigned*) 0x0C200000;
static volatile unsigned* const plic_claim = (unsigned*) 0x0C200004;

void plic_init() {
    plic_enables[0] = 0;
    plic_enables[1] = 0;
    plic_threshold[0] = 0;
}

void plic_enable(unsigned irq) {
    plic_enables[irq/32] = bit_set(plic_enables[irq/32], irq % 32);
}

void plic_set_priority(unsigned irq, unsigned priority) {
    plic_priority[irq] = priority;
}

void plic_claim_clear() {
    plic_claim[0] = plic_claim[0];
}

void exception_init(void (*handler)()) {
    // enable external interrupts
    write_csr(mie, (1 << 11));
    // enable interrupts
    write_csr(mstatus, (1 << 3));
    write_csr(mtvec, handler);
}
