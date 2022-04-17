#pragma once

void exception_init(void (*handler)());
void plic_init();
void plic_enable(unsigned irq);
void plic_set_priority(unsigned irq, unsigned priority);
void plic_claim_clear();
