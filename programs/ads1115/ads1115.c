#include <stdint.h>

#include "libfive/libfive.h"

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

#define MODE_V(x) bits_get(x, 8, 8)
#define DR_V(x) bits_get(x, 5, 7)
#define PGA_V(x) bits_get(x, 9, 11)

static uint8_t ads1115_config() {
    enum { dev_addr = 0b1001000 };

    ads1115_reset();

    delay_us(15);

    uint16_t c = ads1115_read16(dev_addr, config_reg);

    if (!(MODE_V(c) == 1 && DR_V(c) == 0b100 && PGA_V(c) == 0b010)) {
        printf("ERROR: config has incorrect default value: %x\n", c);
        halt();
    }

    c = bits_set(c, 8, 8, 0); // MODE
    c = bits_set(c, 5, 7, 0b111); // DR
    c = bits_set(c, 9, 11, 0b001); // PGA
    c = bits_set(c, 12, 14, 0b100); // MUX

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
