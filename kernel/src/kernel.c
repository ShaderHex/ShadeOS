#include "drivers/display.h"
#include "core/software/shell.h"
#include "drivers/panic.h"
#include "drivers/hwinfo.h"
#include <limine.h>


void kmain() {
    init_framebuffer();

    if (memmap_request.response == NULL) {
        print_string("Memmap not available!\n", 0xFFFFFFFF);
        kpanic("NO MEMMAP");
    }

    print_memmap();
    print_string("\n", 0xFFFFFFFF);
    print_memory_info_detailed();
    print_memory_info();

    print_string("ShadeOS x86_64 Pre-release!\n", 0xFFFFFFFF);

    shell_loop();

    for (;;) {
        asm("hlt");
    }
}
