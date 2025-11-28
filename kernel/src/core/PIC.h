#pragma once
#include <stdint.h>
#include "../drivers/io.h"
#include "../drivers/display.h"

void PIC_sendEOI(uint8_t irq);
void PIC_remap(int offset1, int offset2);
void PIC_disable(void);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);