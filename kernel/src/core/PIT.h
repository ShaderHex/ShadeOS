#pragma once

#include <stdint.h>
#include <stddef.h>

#include "../drivers/io.h"
#include "../drivers/display.h"
#include "irq.h"

void pit_init(uint32_t frequency);
void pic_remap();
uint64_t get_ticks();
uint64_t get_seconds(uint32_t freq);