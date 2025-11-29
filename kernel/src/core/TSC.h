#pragma once
#include <stdint.h>
#include "../drivers/panic.h"

extern uint64_t current_fps;

void tsc_init(void);
void fps_tick(void);
