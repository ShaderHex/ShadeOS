#pragma once
#include <stdint.h>
#include "../drivers/io.h"
#include "../drivers/panic.h"


extern uint32_t IRQ0_fractions;
extern uint32_t IRQ0_ms;
extern uint32_t system_timer_fractions;
extern uint32_t system_timer_ms;

extern uint16_t IRQ0_frequency;
extern uint16_t PIT_reload_value;

uint64_t init_countdown();
unsigned read_pit_count();
void set_pit_count(unsigned count);
void sleep(uint32_t millis);