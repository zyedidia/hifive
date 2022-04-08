#include "libfive/riscv_csr.h"

void exception_init() {
    // enable external interrupts
    write_csr(mie, (1 << 11));
    // enable interrupts
    write_csr(mstatus, (1 << 3));
}

void _empty_exception(unsigned mcause) { (void) mcause; while (1) {} }

void set_exception_handler(void (*handler)(unsigned)) {
    _exception_handler = handler;
}
