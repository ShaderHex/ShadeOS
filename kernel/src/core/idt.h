#pragma once
#include <stdint.h>
#include <stdbool.h>

#define IDT_MAX_DESCRIPTORS sizeof(idt_entry_t)

typedef struct {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];


void exception_handler(void);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);