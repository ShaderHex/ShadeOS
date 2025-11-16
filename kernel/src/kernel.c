#include "drivers/display.h"

void kmain() {
    init_framebuffer();

    clear_screen();

    print_string(8, 8, "Hello, I don't recommend touching my PC!", 0xFFFFFFFF);
    print_string(336, 8, "Stay away from it...\nThank you!", 0xFFFFFFFF);

    for (;;) {
        asm("hlt");
    }
}
