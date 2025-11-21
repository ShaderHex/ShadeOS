#include "idt.h"

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__ ((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__ ((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void irq0_stub();

void idt_set_gate(int n, uint64_t handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].ist = 0;
    idt[n].type_attr = 0x8E;
    idt[n].offset_mid = (handler >> 16) & 0xFFFF;
    idt[n].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[n].zero = 0;
}

void idt_init() {
    idtp.limit = sizeof(idt) -1;
    idtp.base = (uint64_t)&idt;
    idt_set_gate(32, (uint64_t)irq0_stub);
    asm volatile("lidt %0" : : "m"(idtp));
}