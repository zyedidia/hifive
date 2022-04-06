#pragma once

#include <stdint.h>

#include "libfive/bits.h"

typedef enum {
    REG_ZERO,
    REG_RA,
    REG_SP,
    REG_GP,
    REG_TP,
    REG_T0,
    REG_T1,
    REG_T2,
    REG_S0,
    REG_S1,
    REG_A0,
    REG_A1,
    REG_A2,
    REG_A3,
    REG_A4,
    REG_A5,
    REG_A6,
    REG_A7,
    REG_S2,
    REG_S3,
    REG_S4,
    REG_S5,
    REG_S6,
    REG_S7,
    REG_S8,
    REG_S9,
    REG_S10,
    REG_S11,
    REG_T3,
    REG_T4,
    REG_T5,
    REG_T6,
} reg_t;

static inline uint32_t riscv_addi(uint32_t rd, uint32_t rs1, uint32_t imm) {
    return 0b0010011 | (rd << 7) | (0b000 << 12) | (rs1 << 15) | (imm << 20);
}

static inline uint32_t riscv_ori(uint32_t rd, uint32_t rs1, uint32_t imm) {
    return 0b0010011 | (rd << 7) | (0b110 << 12) | (rs1 << 15) | (imm << 20);
}

static inline uint32_t riscv_auipc(uint32_t rd, uint32_t imm) {
    return 0b0010111 | (rd << 7) | (imm << 12); 
}

static inline uint32_t riscv_lui(uint32_t rd, uint32_t imm) {
    return 0b0110111 | (rd << 7) | (imm << 12);
}

static inline uint32_t riscv_lw(uint32_t rd, uint32_t rs1, uint32_t imm) {
    return 0b0000011 | (rd << 7) | (0b010 << 12) | (rs1 << 15) | (imm << 20);
}

static inline uint32_t riscv_sw(uint32_t rs2, uint32_t rs1, uint32_t imm) {
    return 0b0100011 | (bits_get(imm, 0, 4) << 7) | (0b010 << 12) | (rs1 << 15) | (rs2 << 20) | (bits_get(imm, 5, 11) << 25);
}

static inline uint32_t riscv_jal(uint32_t rd, uint32_t addr) {
    return 0b1101111 | (rd << 7) | (bits_get(addr, 12, 19) << 12) | (bit_get(addr, 11) << 20) | (bits_get(addr, 1, 10) << 21) | (bit_get(addr, 20) << 31);
}

static inline uint32_t riscv_jalr(uint32_t rd, uint32_t rs1, uint32_t imm) {
    return 0b1100111 | (rd << 7) | (0b000 << 12) | (rs1 << 15) | (imm << 20);
}

static inline uint32_t riscv_ret() {
    return 0x8067;
}

static inline int riscv_li(uint32_t rd, uint32_t imm, uint32_t* code) {
    code[0] = riscv_lui(rd, bits_get(imm, 12, 31)+1);
    uint32_t rdval = (bits_get(imm, 12, 31)+1) << 12;
    code[1] = riscv_addi(rd, rd, imm - rdval);
    return 2;
}
