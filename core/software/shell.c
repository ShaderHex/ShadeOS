#include "../../drivers/display.h"
#include "../../drivers/keyboard.h"
#include "shell.h"

void shell_loop(void) {
    print_string("Welcome to the shell!\n\n");
    int isRunning = 1;

    while (isRunning) {
        unsigned char status = port_byte_in(0x64);
        if (status & 0x01) {
            unsigned char scancode = keyboard_read_scancode();
            char ch = keyboard_scancode_to_char(scancode);
            if (ch != 0) {
                char str[2] = { ch, 0 };
                print_string("");
                print_string(str);
            }
        }
    }
}
