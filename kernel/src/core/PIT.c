#include "PIT.h"

#include <stdint.h>

uint32_t IRQ0_fractions = 0;
uint32_t IRQ0_ms = 0;
uint32_t system_timer_fractions = 0;
uint32_t system_timer_ms = 0;

uint16_t IRQ0_frequency = 1000;
uint16_t PIT_reload_value = 1193;

volatile uint64_t CountDown = 0;

uint64_t init_countdown() {
    volatile uint64_t CountDown = 0;

    return CountDown;
}

unsigned read_pit_count() {
    unsigned count = 0;
    asm("cli");
    
    port_byte_out(0x43, 0b0000000);

    count = port_byte_in(0x40);
    count |= port_byte_in(0x40)<<8;
    
    return count;
}

void set_pit_count(unsigned count) {
    asm("cli");

    port_byte_out(0x40, count&0xFF);
    port_byte_out(0x40, (count&0xFF00)>>8);
    return;
}

void sleep(uint32_t millis) {
    asm("sti");
    CountDown = millis;
    while (CountDown == 0) {
        asm("hlt");
    }
}
