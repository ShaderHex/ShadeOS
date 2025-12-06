#pragma once
#include <stdint.h>
#include "../drivers/panic.h"

extern uint64_t current_fps;

uint64_t rdtsc();
void tsc_init(void);
void fps_tick(void);
