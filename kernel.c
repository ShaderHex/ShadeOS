#include <stdint.h>
#include "drivers/display.h"
#include "drivers/keyboard.h"
#include "core/software/shell.h"

uint32_t *kernel_load_addr = (uint32_t *)0x10000;

int main() {
    *kernel_load_addr = 0x10000;
    clear_screen();
    print_string("Kernel Mapped at: ", 0x0f);
    print_hex(*kernel_load_addr);
    print_string("\n", 0x0f);
    print_string("Welcome to ShadeOS!\n", 0x0f);
    init_keyboard();
    keyboard_enable();
    shell_loop();
    return 0;
}
