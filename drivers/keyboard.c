#include "keyboard.h"

void init_keyboard() {
    print_string("Initializing keyboard driver...\n");
    unsigned char scancode = port_byte_in(0x60);
    print_string("Done initializing keyboard driver!");
}

unsigned char keyboard_read_scancode() {
    return port_byte_in(0x60);
}

char keyboard_scancode_to_char(unsigned char scancode) {
    switch (scancode) {
        case 0x1E: return 'a';
        case 0x30: return 'b';
        case 0x2E: return 'c';
        default: return 0;
    }
}