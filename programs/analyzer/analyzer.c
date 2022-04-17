#include "libfive/uart.h"
#include "libfive/gpio.h"
#include "libfive/timer.h"
#include "libfive/riscv_csr.h"
#include "libfive/bits.h"

#include <string.h>

#define PIN 23

typedef struct {
    unsigned ncycles;
    unsigned v;
} event_t;

static volatile unsigned* const gpio_input_val = (unsigned*) 0x10012000;

static inline unsigned gpio_read_fast(unsigned pin) {
    return bit_get(*gpio_input_val, pin);
}

unsigned analyze(event_t* events, size_t N) {
    unsigned n = 0;
    while (n < N) {
        unsigned v1, v0 = gpio_read_fast(PIN);
        while ((v1 = gpio_read_fast(PIN)) == v0) {}
        unsigned time = read_csr(mcycle);
        events[n++] = (event_t){
            .ncycles = time,
            .v = v1,
        };
        v0 = v1;
    }
    return n;
}

unsigned analyze_no_br(event_t* events, size_t N) {
    unsigned n = 0;

    unsigned v1, v0 = gpio_read_fast(PIN);
    unsigned time = read_csr(mcycle);
    while ((v1 = gpio_read_fast(PIN)) == v0) {
        time = read_csr(mcycle);
    }
    events[n++] = (event_t){
        .ncycles = time,
        .v = v1,
    };
    v0 = v1;

    while (n < N) {
        unsigned time = read_csr(mcycle);
        v1 = gpio_read_fast(PIN);
        events[n] = (event_t){
            .ncycles = time,
            .v = v1,
        };
        n += v1 != v0;
        v0 = v1;
    }

    events[1].ncycles -= 40;

    return n;
}

unsigned analyze_inner(event_t* events, size_t N) {
    unsigned n = 0;

    unsigned v1, v0 = gpio_read_fast(PIN);
    unsigned time = read_csr(mcycle);
    while ((v1 = gpio_read_fast(PIN)) == v0) {
        time = read_csr(mcycle);
    }
    events[n++] = (event_t){
        .ncycles = time,
        .v = v1,
    };
    v0 = v1;

#define NREAD 1024
    unsigned readings[NREAD];
    unsigned* start = readings;
    const unsigned* end = readings + NREAD;

    while (n < N) {
        unsigned before = read_csr(mcycle);
        for ( ; start != end; start++) {
            *start = *gpio_input_val;
        }
        unsigned after = read_csr(mcycle);
        unsigned per_cycle = (after - before) / NREAD;
        start -= NREAD;

        for (int i = 0; i < NREAD; i++) {
            if (bit_get(readings[i], PIN) != v0) {
                v0 = !v0;
                events[n++] = (event_t){
                    .ncycles = before + i * per_cycle,
                    .v = v0,
                };
            }
        }
    }
    return n;
}

#define N 8

static int abs(int i) {
    if (i < 0) {
        return -i;
    }
    return i;
}

int main() {
    gpio_set_input(PIN);

    event_t events[N];

    while (1) {
        unsigned n = analyze_no_br(events, N);

        printf("%d results\n", n);

        unsigned start = events[0].ncycles;
        int toterr = 0;
        for (int i = 0; i < n; i++) {
            unsigned got = (events[i].ncycles - start) * 1000 / CPU_FREQ_MHZ;
            unsigned want = i * 6000 * 1000 / 700;
            int err = abs((int) (want - got));
            toterr += err;
            printf("got: %d ns, (err %d ns, toterr %d ns)\n", (events[i].ncycles - start) * 1000 / CPU_FREQ_MHZ, err, toterr);
        }

        printf("toterr: %d pi cycles\n", toterr * 700 / 1000);

        delay_ms(100);

        memset(events, 0, sizeof(event_t) * N);
    }

    return 0;
}
