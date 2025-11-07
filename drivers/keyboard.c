#include "keyboard.h"

static int keyboard_enabled = 1;

void keyboard_enable() {
    keyboard_enabled = 1;
}

void keyboard_disable() {
    keyboard_enabled = 0;
}

void init_keyboard() {
    print_string("Initializing keyboard driver...\n", 0x0f);
    port_byte_in(0x60);
    print_string("Done initializing keyboard driver!\n", 0x0f);
}

unsigned char keyboard_read_scancode() {
    if (keyboard_enabled == 0) {
        return 0;
    } else {
        return port_byte_in(0x60);
    }
}

char keyboard_scancode_to_char(unsigned char scancode) {
    if (keyboard_enabled == 0) {
        return 0;
    } else {
        switch (scancode) {
            case 0x1E: return 'a';
            case 0x30: return 'b';
            case 0x2E: return 'c';
            case 0x20: return 'd';
            case 0x12: return 'e';
            case 0x21: return 'f';
            case 0x22: return 'g';
            case 0x23: return 'h';
            case 0x17: return 'i';
            case 0x24: return 'j';
            case 0x25: return 'k';
            case 0x26: return 'l';
            case 0x32: return 'm';
            case 0x31: return 'n';
            case 0x18: return 'o';
            case 0x19: return 'p';
            case 0x10: return 'q';
            case 0x13: return 'r';
            case 0x1F: return 's';
            case 0x14: return 't';
            case 0x16: return 'u';
            case 0x2F: return 'v';
            case 0x11: return 'w';
            case 0x2D: return 'x';
            case 0x15: return 'y';
            case 0x2C: return 'z';
            case 0x39: return ' ';
            case 0x1C: return '\n';
            case 0x02: return '1';
            default: return 0;
        }
    }
}
