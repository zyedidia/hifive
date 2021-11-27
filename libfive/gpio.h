#pragma once

void gpio_set_output(unsigned pin);
void gpio_set_input(unsigned pin);
void gpio_write(unsigned pin, unsigned val);
void gpio_set_xor(unsigned pin, unsigned val);
unsigned gpio_read(unsigned pin);
