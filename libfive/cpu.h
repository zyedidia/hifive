#pragma once

#define CPU_FREQ 320000000
#define CPU_FREQ_MHZ (CPU_FREQ / (1000 * 1000))

unsigned cpu_freq();
void cpu_clock_init();
