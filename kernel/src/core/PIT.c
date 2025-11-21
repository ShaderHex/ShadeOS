#include "PIT.h"

volatile uint64_t ticks = 0;

void pit_irq_handler() {
    print_string("pit_irq_handler called!\n", 0xFFFFFFFF);
    ticks++;
}

void pit_init(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;

    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, divisor & 0xFF);
    port_byte_out(0x40, divisor >> 8);

    set_irq_handler(0, pit_irq_handler);

    print_string("PIT initialized!", 0xFFFFFFFF);
}

void pic_remap() {
    uint8_t a1, a2;

    a1 = port_byte_in(0x21);
    a2 = port_byte_in(0xA1);

    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);

    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);

    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);

    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    port_byte_out(0x21, a1);
    port_byte_out(0xA1, a2);
}


void pic_eoi() {
    port_byte_out(0x20, 0x20);
}

uint64_t get_ticks() {
    return ticks;
}

uint64_t get_seconds(uint32_t freq) {
    return (double)ticks / (double)freq;
}