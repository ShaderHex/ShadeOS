#include "drivers/display.h"
#include "core/software/shell.h"
#include "core/PIT.h"
#include "core/idt.h"
#include "drivers/panic.h"
#include "drivers/hwinfo.h"
#include <limine.h>

void delay(int count) {
    for (volatile int i = 0; i < count; i++);
}

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
    //pit_init(1000);    

    print_string("ShadeOS x86_64 Pre-release!\n", 0xFFFFFFFF);

    
    pic_remap();
    idt_init();
    pit_init(1000);
    asm volatile("sti");
    uint64_t time = 0;
    char buf[64];
    while (true) {
        uint64_t t = get_ticks();
        int_to_string(t, buf);
        print_string(buf, 0xFFFFFFFF);
        print_string("\n", 0xFFFFFFFF);
        delay(5000000);
    }

    shell_loop();

    for (;;) {
        asm("hlt");
    }
}
