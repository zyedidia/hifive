#include "libfive/riscv_csr.h"
#include "libfive/uart.h"

static void empty_exception(unsigned mcause) { (void) mcause; while (1) {} }

static void (*exception_handler)(unsigned) = empty_exception;

static void exception() {
    exception_handler(read_csr(mcause));
}

void set_exception_handler(void (*handler)(unsigned)) {
    exception_handler = handler;
}

void exception_init() {
    // enable external interrupts
    write_csr(mie, (1 << 11));
    // enable interrupts
    write_csr(mstatus, (1 << 3));
    write_csr(mtvec, exception);
}
