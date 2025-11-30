#pragma once
#include "io.h"
#include "display.h"
#include <stdint.h>

void kpanic_memcpy(void* src, void* dst, size_t n);
void kpanic(const char* msg);