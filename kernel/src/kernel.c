#include "drivers/display.h"

void kmain() {
    init_framebuffer();

    clear_screen();

    print_string(8, 8, "Hello from My Kernel!", 0xFFFFFFFF);
    print_string(8, 16, "Framebuffer text works!", 0xFFFFFFFF);

    for (;;) {
        asm("hlt");
    }
}
