#include "gpio.h"
#include "bits.h"

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

void gpio_set_output(unsigned pin) {
    gpio->output_en = bit_set(gpio->output_en, pin);
    gpio->input_en = bit_clr(gpio->input_en, pin);
}

void gpio_set_input(unsigned pin) {
    gpio->output_en = bit_clr(gpio->output_en, pin);
    gpio->input_en = bit_set(gpio->input_en, pin);
}

void gpio_set_pullup(unsigned pin) {
    gpio->pue = bit_set(gpio->pue, pin);
}

void gpio_write(unsigned pin, unsigned val) {
    gpio->output_val = bit_assign(gpio->output_val, pin, val);
}

void gpio_set_xor(unsigned pin, unsigned val) {
    gpio->out_xor = bit_assign(gpio->out_xor, pin, val);
}

unsigned gpio_read(unsigned pin) {
    return bit_get(gpio->input_val, pin);
}

void gpio_configure(unsigned pin, gpio_iof_t mode) {
    switch (mode) {
        case GPIO_PWM:
            gpio->iof_en = bit_set(gpio->iof_en, pin);
            gpio->iof_sel = bit_set(gpio->iof_en, pin);
            break;
        case GPIO_SPI:
        case GPIO_I2C:
            gpio->iof_en = bit_set(gpio->iof_en, pin);
            gpio->iof_sel = bit_clr(gpio->iof_sel, pin);
            break;
    }
}
