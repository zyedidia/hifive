#pragma once

#include <stdbool.h>

typedef enum {
    GPIO_PWM,
    GPIO_SPI,
    GPIO_I2C,
} gpio_iof_t;

void gpio_set_output(unsigned pin);
void gpio_set_input(unsigned pin);
void gpio_set_pullup(unsigned pin);
void gpio_write(unsigned pin, unsigned val);
void gpio_set_xor(unsigned pin, unsigned val);
unsigned gpio_read(unsigned pin);
void gpio_configure(unsigned pin, gpio_iof_t mode);

unsigned gpio_irq_num(unsigned pin);
void gpio_enable_irq(unsigned pin);
bool gpio_irq_fall(unsigned pin);
bool gpio_irq_rise(unsigned pin);
void gpio_irq_clear(unsigned pin);
