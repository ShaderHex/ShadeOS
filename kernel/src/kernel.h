#pragma once
#include <stdint.h>

typedef struct {
    uint32_t kernel_load_addr;
} KernelInfo;

extern KernelInfo g_kernel_info;