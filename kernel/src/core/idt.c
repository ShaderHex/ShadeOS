#include "idt.h"

void exception_handler() {
    kpanic("IDT Fault!");
    __asm__ volatile ("cli; hlt");
}

void fps_exception_handler(void* frame) {
    kpanic("CPU Exception caught during FPS tick!");
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }
    idt_set_descriptor(0, fps_exception_handler, 0x8E);   // Divide Error
    idt_set_descriptor(7, fps_exception_handler, 0x8E);   // Device Not Available (SSE/FPU)
    idt_set_descriptor(13, fps_exception_handler, 0x8E);  // General Protection

    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");
}