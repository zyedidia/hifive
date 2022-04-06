#include "riscv.h"
#include "libfive/uart.h"
#include "libfive/timer.h"

int count;

void int1() { count += 1; }
void int2() { count += 2; }
void int3() { count += 3; }
void int4() { count += 4; }
void int5() { count += 5; }
void int6() { count += 6; }
void int7() { count += 7; }

typedef void (*intfn_t)();

void hello() {
    printf("Hello world\n");
}

#define BENCH(call) \
{ int og_tx = read_ticks(); \
    for (int _i = 0; _i < 10000; _i++) {call;} \
  printf("tx: %d\n", read_ticks() - og_tx); }

void slow(intfn_t* fns, int n) {
    for (int i = 0; i < n; i++) {
        fns[i]();
    }
}

void fast(intfn_t* fns, int nfn) {
    uint32_t code[100];
    int n = 0;

    code[n++] = riscv_addi(REG_SP, REG_SP, -16);
    code[n++] = riscv_sw(REG_RA, REG_SP, 12);

    /* n += riscv_li(REG_T0, (uint32_t) hello, &code[n]); */
    /* code[n++] = riscv_jalr(REG_RA, REG_T0, 0); */

    for (int i = 0; i < nfn; i++) {
        n += riscv_li(REG_T0, (uint32_t) fns[i], &code[n]);
        code[n++] = riscv_jalr(REG_RA, REG_T0, 0);
    }

    code[n++] = riscv_lw(REG_RA, REG_SP, 12);
    code[n++] = riscv_addi(REG_SP, REG_SP, 16);
    code[n++] = riscv_ret();

    printf("%lx\n", code[0]);
    printf("%lx\n", code[1]);
    printf("%lx\n", code[2]);
    printf("%lx\n", code[3]);
    printf("%lx\n", code[4]);
    printf("%lx\n", code[5]);
    printf("%lx\n", code[6]);
    printf("%lx\n", code[7]);

    for (int i = 0; i < 32; i++) {
        asm volatile ("nop");
    }

    void (*fn)(void) = (typeof(fn)) code;
    BENCH(fn());
}

int main() {
    intfn_t fns[] = {
        int1,
        int2,
        int3,
        int4,
        int5,
        int6,
        int7,
    };

    /* BENCH(slow(fns, 7)); */
    fast(fns, 7);
    printf("count: %d\n", count);

    return 0;
}
