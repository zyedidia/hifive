#include "i2c.h"
#include "bits.h"
#include "gpio.h"
#include "mem.h"
#include "uart.h"

typedef struct {
    // Clock prescale low
    uint8_t prerlo;
    uint16_t _pad1;
    // Clock prescale high
    uint8_t prerhi;
    uint16_t _pad2;
    // Control register
    uint8_t ctr;
    uint16_t _pad3;
    // Transmit/Receive register
    uint8_t txr_rxr;
    uint16_t _pad4;
    // Command/statis register
    uint8_t cr_sr;
    uint16_t _pad5;
} i2c_reg_t;

typedef enum {
    CTR_EN = 7,
} i2c_ctr_t;

typedef enum {
    CR_STA = 7,
    CR_STO = 6,
    CR_RD = 5,
    CR_WR = 4,
    CR_ACK = 3,
} i2c_cr_t;

typedef enum {
    SR_ACK = 7,
    SR_TIP = 1,
} i2c_sr_t;

static volatile i2c_reg_t* const i2c = (volatile i2c_reg_t*) 0x10016000;
static const uint32_t i2c_clock_freq = 32000000;

void i2c_setup(uint32_t freq, int scl, int sda) {
    uint32_t prescaler = i2c_clock_freq / (5 * freq) - 1;
    // clear control reg before setting prescale
    i2c->ctr = bit_clr(i2c->ctr, CTR_EN);
    // set prescaler registers
    i2c->prerlo = (uint8_t) (prescaler & 0xff);
    i2c->prerhi = (uint8_t) ((prescaler >> 8) & 0xff);
    // enable controller
    i2c->ctr = bit_set(i2c->ctr, CTR_EN);

    gpio_configure(scl, GPIO_I2C);
    gpio_configure(sda, GPIO_I2C);
}

static inline int i2c_ready() {
    // transmission in progress bit
    uint8_t tip = bit_get(i2c->cr_sr, SR_TIP);
    return tip == 0;
}

static void i2c_start() {
    while (!i2c_ready()) {
    }
    i2c->cr_sr = bit_set(0, CR_STA);
}

static void i2c_stop() {
    while (!i2c_ready()) {
    }
    i2c->cr_sr = bit_set(0, CR_STO);
}

static int i2c_write_byte(uint8_t data) {
    i2c->txr_rxr = data;
    i2c->cr_sr = bit_set(0, CR_WR);
    while (!i2c_ready()) {
    }
    if (bit_get(i2c->cr_sr, SR_ACK) == 1) {
        // NACK
        return -1;
    }
    return 0;
}

static int i2c_read_byte() {
    i2c->cr_sr = bit_set(0, CR_RD);
    while (!i2c_ready()) {
    }
    uint8_t byte = i2c->txr_rxr;
    return byte;
}

static void send_addr(uint8_t addr, int write) {
    uint8_t data = addr << 1;
    if (!write) {
        data |= 1;
    }
    i2c->txr_rxr = data;
    uint8_t cr = 0;
    cr = bit_set(cr, CR_STA);
    cr = bit_set(cr, CR_WR);
    i2c->cr_sr = cr;
    while (!i2c_ready()) {
    }
}

int i2c_read(uint8_t dev, volatile uint8_t* bytes, size_t num) {
    if (num == 0) {
        return 0;
    }

    send_addr(dev, 0);
    if (bit_get(i2c->cr_sr, SR_ACK) == 1) {
        return -1;
    }
    bytes[0] = i2c_read_byte();
    for (size_t i = 1; i < num; i++) {
        // send ACK
        i2c->cr_sr = bit_clr(0, CR_ACK);
        bytes[i] = i2c_read_byte();
    }
    // send NACK
    i2c->cr_sr = bit_set(0, CR_ACK);
    i2c_stop();
    return 0;
}

int i2c_write(uint8_t dev, volatile uint8_t* bytes, size_t num, int rstart) {
    send_addr(dev, 1);
    if (bit_get(i2c->cr_sr, SR_ACK) == 1) {
        return -1;
    }
    for (size_t i = 0; i < num; i++) {
        int err = i2c_write_byte(bytes[i]);
        if (err < 0) {
            return -1;
        }
    }
    if (!rstart) {
        i2c_stop();
    }
    return 0;
}
