#pragma once
#include<stdint.h>
#include "../drivers/display.h"

typedef enum {
    SYSCALL_PRINT_STRING,
    SYSCALL_GET_KERNEL_ADDR,
    SYSCALL_GET_MEMORY_MAP,
} SyscallID;

uint32_t syscall(SyscallID id, uint32_t arg0, uint32_t arg1, uint32_t arg2);