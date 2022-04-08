#include <stdbool.h>

#include "libfive/timer.h"
#include "libfive/gpio.h"
#include "libfive/uart.h"

const static unsigned ir_eps = 200;

typedef struct readings {
    unsigned usec, v;
} readings_t;

static char* num_to_str(unsigned x) {
    switch (x) {
    case 0xff00bf00:
        return "power";
    case 0xf708bf00:
        return "down";
    case 0xfb04bf00:
        return "backward";
    case 0xfa05bf00:
        return "pause/play";
    case 0xf906bf00:
        return "forward";
    default:
        return "unknown";
    }
}

static unsigned read_while_eq(int pin, int v, unsigned timeout) {
    unsigned rb = read_us();
    while (gpio_read(pin) == v) {
        unsigned ra = read_us();
        if ((ra - rb) >= timeout) {
            return 0;
        }
    }
    return read_us() - rb;
}


static bool near(unsigned usec, unsigned val) {
    return usec > val-ir_eps && usec < val+ir_eps;
}

// return 1 if is a skip: skip = delay of 550-/+eps
static bool is_skip(struct readings *e) {
    return e->v == 0 && near(e->usec, 550);
}

// integer absolute value.
static int abs(int x) {
    return x < 0 ? -x : x;
}

// return 0 if e is closer to <lb>, 1 if its closer to <ub>
static int pick(struct readings *e, unsigned lb, unsigned ub) {
    int low = abs((int) e->usec - (int) lb);
    int high = abs((int) e->usec - (int) ub);

    return low > high;
}

// header is a delay of 9000 and then a delay of 4500
static bool is_header(struct readings *r, unsigned n) {
    if(n < 2)
        return false;
    if ((r[0].v == 0 && near(r[0].usec, 9000)) &&
        (r[1].v == 1 && near(r[1].usec, 4500))) {
            return true;
    }
    return false;
}

static unsigned convert(readings_t* r, unsigned n) {
    if (!is_header(&r[0], n)) {
        return 0;
    }
    unsigned result = 0;
    unsigned s = 0;
    for (int i = 2; i < n; i += 2) {
        if (!is_skip(&r[i])) {
            return result;
        }
        int bit = pick(&r[i+1], 600, 1600);
        result |= bit << (s++);
    }

    return result;
}

static int read(int pin, readings_t* r, unsigned n) {
    int v = 0;
    int i;
    for (i = 0; i < n; i++) {
        r[i].usec = read_while_eq(pin, v, 15000);
        r[i].v = v;
        v = !v;
        if (r[i].usec < 10) {
            break;
        }
    }
    return i;
}

int main() {
    // pin '7' on board
    const int pin = 23;
    gpio_set_input(pin);
    gpio_set_pullup(pin);

    const int N = 256;
    readings_t r[N];

    while (1) {
        while (gpio_read(pin)) {}

        int n = read(pin, r, N);

        if (!is_header(r, n)) {
            continue;
        }
        unsigned x = convert(r, n);
        printf(num_to_str(x));
    }

    return 0;
}
