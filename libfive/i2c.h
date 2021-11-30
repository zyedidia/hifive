#pragma once

#include <stddef.h>
#include <stdint.h>

void i2c_setup(uint32_t freq, int scl, int sda);
int i2c_read(uint8_t dev, uint8_t* bytes, size_t num);
int i2c_write(uint8_t dev, uint8_t* bytes, size_t num, int rstart);
