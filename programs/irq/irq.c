#include "libfive/uart.h"
#include "libfive/exception.h"
#include "libfive/gpio.h"
#include "libfive/riscv_csr.h"
#include "libfive/timer.h"
#include "libfive/bits.h"

#define PIN 23

typedef struct {
    unsigned input_val;
    unsigned input_en;
    unsigned output_en;
    unsigned output_val;
    unsigned pue;
    unsigned ds;
    unsigned rise_ie;
    unsigned rise_ip;
    unsigned fall_ie;
    unsigned fall_ip;
    unsigned high_ie;
    unsigned high_ip;
    unsigned low_ie;
    unsigned low_ip;
    unsigned iof_en;
    unsigned iof_sel;
    unsigned out_xor;
} gpio_reg_t;

static volatile gpio_reg_t* const gpio = (gpio_reg_t*) 0x10012000;

static volatile unsigned* const plic_claim = (unsigned*) 0x0C200004;

typedef struct {
    unsigned ncycles;
    unsigned v;
} event_t;

#define N 8

static event_t events[N];
static int n;

void __attribute__((aligned(16), naked)) irq_handler_asm() {
    asm volatile (
        "csrr t6, mcycle\n"
        "csrw mscratch, t6\n"
        "j irq_handler\n"
    );
}

void __attribute__((interrupt)) irq_handler() {
    unsigned cyc = read_csr(mscratch);
    if (n < N) {
        events[n++] = (event_t){
            .ncycles = cyc,
            .v = bit_get(gpio->rise_ip, PIN),
        };
    }
    gpio->rise_ip = bit_set(gpio->rise_ip, PIN);
    gpio->fall_ip = bit_set(gpio->fall_ip, PIN);
    *plic_claim = *plic_claim;
}

static int abs(int i) {
    if (i < 0) {
        return -i;
    }
    return i;
}

int main() {
    n = 0;

    unsigned pin = PIN; // pin marked '4'

    plic_init();
    plic_enable(gpio_irq_num(pin));
    plic_set_priority(gpio_irq_num(pin), 7);

    exception_init(irq_handler_asm);
    gpio_set_input(pin);
    gpio_enable_irq(pin);

    while (n < N) {
        asm volatile ("wfi");
    }

    unsigned start = events[0].ncycles;
    int toterr = 0;
    for (int i = 0; i < n; i++) {
        unsigned got = (events[i].ncycles - start) * 1000 / CPU_FREQ_MHZ;
        unsigned want = i * 6000 * 1000 / 700;
        int err = abs((int) (want - got));
        toterr += err;
        printf("%d: got: %d ns, (err %d ns, toterr %d ns)\n", events[i].v, (events[i].ncycles - start) * 1000 / CPU_FREQ_MHZ, err, toterr);
    }

    printf("toterr: %d pi cycles\n", toterr * 700 / 1000);

    return 0;
}
