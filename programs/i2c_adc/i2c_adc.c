#include <stdint.h>

#include "libfive/uart.h"
#include "libfive/bits.h"
#include "libfive/i2c.h"
#include "libfive/gpio.h"
#include "libfive/timer.h"

enum { conversion_reg = 0, config_reg = 1 };

static void ads1115_reset() {
    uint8_t rst = 0x6;
    i2c_write(0, &rst, 1, 0);
}

static void ads1115_write16(uint8_t dev_addr, uint8_t reg, uint16_t v) {
    uint8_t data[3];
    data[0] = reg;
    data[1] = v >> 8;
    data[2] = v & 0xff;
    i2c_write(dev_addr, data, 3, 0);
}

// read a 16-bit register
static uint16_t ads1115_read16(uint8_t dev_addr, uint8_t reg) {
    i2c_write(dev_addr, &reg, 1, 1);
    uint8_t data[2];
    i2c_read(dev_addr, data, 2);
    return (data[0] << 8) | data[1];
}

#define PGA(x) ((x) << (9))
#define MODE(x) ((x) << (8))
#define DR(x) ((x) << 5)
#define MUX(x) ((x) << 12)

#define MODE_V(x) bits_get(x, 8, 8)
#define DR_V(x) bits_get(x, 5, 7)
#define PGA_V(x) bits_get(x, 9, 11)
#define MUX_V(x) bits_get(x, 12, 14)

static uint8_t ads1115_config() {
    enum { dev_addr = 0b1001000 };

    ads1115_reset();

    delay_us(15);

    uint16_t c = ads1115_read16(dev_addr, config_reg);

    printf("config: %x\n", c);

    c = bits_set(c, 8, 8, 0);
    c = bits_set(c, 5, 7, 0b111);
    c = bits_set(c, 9, 11, 0b001);
    c = bits_set(c, 12, 14, 0b100);

    ads1115_write16(dev_addr, config_reg, c);

    return dev_addr;
}

int main() {
    i2c_setup(100000, 13, 12);

    delay_us(20);

    uint8_t dev_addr = ads1115_config();

    while (1) {
        uint16_t v = ads1115_read16(dev_addr, conversion_reg);
        printf("reading: %d\n", v);
        delay_ms(500);
    }

    return 0;
}
