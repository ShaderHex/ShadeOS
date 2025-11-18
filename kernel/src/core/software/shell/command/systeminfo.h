#pragma once
#include "../../../../drivers/display.h"
#include "../../../../drivers/io.h"
#include "../../../../drivers/hwinfo.h"
#include <stdint.h>

typedef unsigned int u32;
typedef unsigned long long u64;

static inline void cpuid(u32 code, u32* a, u32* b, u32* c, u32* d) {
    __asm__ volatile("cpuid"
                     : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                     : "a"(code));
}

//extern uint32_t kernel_load_addr;

void print_cpu_info() {
    u32 eax, ebx, ecx, edx;
    cpuid(0, &eax, &ebx, &ecx, &edx);

    char vendor[13];
    *((u32*)&vendor[0]) = ebx;
    *((u32*)&vendor[4]) = edx;
    *((u32*)&vendor[8]) = ecx;
    vendor[12] = 0;

    print_string("CPU Vendor: ", 0xFFFFFFFF);
    print_string(vendor, 0xFFFFFFFF);
    print_string("\n", 0xFFFFFFFF);

    cpuid(1, &eax, &ebx, &ecx, &edx);
    u32 cores = ((ebx>>16) & 0xff) + 1;

    char buf[16];
    int_to_string(cores, buf);
    print_string("CPU Cores: ", 0xFFFFFFFF);
    print_string(buf, 0xFFFFFFFF);
    print_string("\n", 0xFFFFFFFF);

    print_string("Features (EDX): 0x", 0xFFFFFFFF);
    for (int i = 28; i >= 0; i -= 4) {
        u32 nibble = (edx >> i) & 0xF;
        char c = nibble < 10 ? '0'+nibble : 'A'+(nibble-10);
        print_string(&c, 0xFFFFFFFF);
    }
    print_string("\n\n", 0xFFFFFFFF);
}

struct E820Entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
};

struct E820Entry* entries = (struct E820Entry*)0x9000;
//uint8_t entries_count = *(volatile uint8_t*)0x8FF0;


void print_kernel_mapped_info() {
    char buf[64];

    print_string("\nKernel Mapped at: ", 0xFFFFFFFF);
    print_string(buf, 0xFFFFFFFF);
    print_string("\n", 0xFFFFFFFF);
}

void cmd_systeminfo() {
    print_string("\n=== SYSTEM INFO ===\n", 0xFFFFFFFF);
    print_cpu_info();
    print_memory_info();
    print_kernel_mapped_info();
    print_string("===================\n", 0xFFFFFFFF);
}