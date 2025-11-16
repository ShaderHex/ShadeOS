#include "panic.h"

#define VGA_MEMORY 0xB8000
#define PANIC_COLOR 0x4F

void kpanic(const char* msg) {
    volatile uint8_t* vga = (volatile uint8_t*)VGA_MEMORY;

    for (int i = 0; i < 80 * 25; i++) {
        vga[i*2] = ' ';
        vga[i*2+1] = PANIC_COLOR;
    }


    print_string("                        !! KERNEL PANIC !!\n\n", PANIC_COLOR);
    print_string("                            Panic Reason: ", PANIC_COLOR);
    print_string(msg, PANIC_COLOR);

    for(;;) {
        __asm__("hlt");
    }
}
