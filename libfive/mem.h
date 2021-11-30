#pragma once

#include <stdint.h>

static inline void put32(volatile void* addr, uint32_t v) {
    asm volatile("fence rw,rw");
    *((volatile uint32_t*) addr) = v;
    asm volatile("fence rw,rw");
}

static inline uint32_t get32(const volatile void* addr) {
    asm volatile("fence rw,rw");
    return *((volatile uint32_t*) addr);
}

static inline uint8_t get8(const volatile void* addr) {
    asm volatile("fence rw,rw");
    return *((volatile uint8_t*) addr);
}

static inline void put8(volatile void* addr, uint8_t v) {
    asm volatile("fence rw,rw");
    *((volatile uint8_t*) addr) = v;
    asm volatile("fence rw,rw");
}

