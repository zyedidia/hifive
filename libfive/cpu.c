#include <stdint.h>

#include "cpu.h"
#include "timer.h"

#define PLLR_2 1
#define PLLQ_2 1

unsigned cpu_freq() {
    return CPU_FREQ;
}

#define PLLCFG          *(volatile uint32_t*) 0x10008008

// PLLCFG register bit indexes
#define PLLR_I        0U
#define PLLF_I        4U
#define PLLQ_I        10U
#define PLLSEL_I      16U
#define PLLREFSEL_I   17U
#define PLLBYPASS_I   18U
#define PLLLOCK_I     31U

// Sets CPU speed to 320 MHz
void cpu_clock_init() {
    uint32_t cfg_temp = 0;

    // There is a 16 MHz crystal oscillator HFXOSC on the board
    cfg_temp |= 1 << PLLREFSEL_I;     // Drive PLL from 16 MHz HFXOSC
    cfg_temp |= PLLR_2 << PLLR_I;     // Divide ratio. R=2 for 8 MHz out
    cfg_temp |= (80/2 - 1U) << PLLF_I;  // Multiply ratio. 8 MHz x 40 is 640 MHz out
    cfg_temp |= PLLQ_2 << PLLQ_I;     // Divide 640MHz with 2 to get 320 MHz
    // PLLSEL_I = 0    : PLL is not driving hfclk for now
    // PLLBYPASS_I = 0 : Enables PLL
    PLLCFG = cfg_temp;

    while ((PLLCFG & (1 << PLLLOCK_I)) == 0) {} // Wait until PLL locks
    PLLCFG |= 1 << PLLSEL_I;          // Let PLL drive hfclk
}
