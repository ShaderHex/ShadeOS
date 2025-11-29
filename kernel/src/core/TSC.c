#include <stdint.h>
#include "TSC.h"

uint64_t last_time;
uint64_t tsc_per_sec = 3400000000ULL;
uint64_t current_fps;

static inline uint64_t rdtsc() {
    unsigned int hi, lo;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

void tsc_init() {
    last_time = rdtsc();
}

void fps_tick() {
    uint64_t now = rdtsc();
    uint64_t delta = now - last_time;
    last_time = now;

    if (delta == 0) {
        kpanic("FPS tick delta=0! Simulated CPU exception");
    }

    current_fps = tsc_per_sec / delta;
}
