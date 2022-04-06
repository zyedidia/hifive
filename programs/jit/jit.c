#include <stdint.h>

#include "libfive/uart.h"
#include "riscv.h"

int main() {
    char* str = "hello world\n";

    uint32_t code[32];

    int n = 0;
    code[n++] = riscv_addi(REG_SP, REG_SP, -16);
    n += riscv_li(REG_A0, (uint32_t) str, &code[n]);
    code[n++] = riscv_sw(REG_RA, REG_SP, 12);
    n += riscv_li(REG_T0, (uint32_t) tfp_printf, &code[n]);
    code[n++] = riscv_jalr(REG_RA, REG_T0, 0);
    code[n++] = riscv_lw(REG_RA, REG_SP, 12);
    code[n++] = riscv_addi(REG_SP, REG_SP, 16);
    code[n++] = riscv_ret();

    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");

    void (*fn)(void) = (void (*)(void)) code;

    fn();

    printf("done\n");
    return 0;
}
