#include "drivers/display.h"
#include "core/software/shell.h"
#include "core/PIT.h"
#include "core/idt.h"
#include "core/gdt.h"
#include "core/TSC.h"
#include "drivers/panic.h"
#include "drivers/hwinfo.h"
#include <limine.h>

void delay(int count) {
    for (volatile int i = 0; i < count; i++);
}

void kmain() {
    heap_init();
    init_framebuffer();

    if (memmap_request.response == NULL) {
        print_string("Memmap not available!\n", 0xFFFFFFFF);
        kpanic("NO MEMMAP");
    }

    print_memmap();
    print_string("\n", 0xFFFFFFFF);
    print_memory_info_detailed();
    print_memory_info();    
    tsc_init();
    print_debug("Test\n", 1, 0);
    //idt_init();
    //gdt_init();
    asm("sti");

    print_string("\nShadeOS x86_64 Pre-release!\n", 0xFFFFFFFF);
    
    //uint64_t time = read_pit_count();  
    while(false) {
        //fps_tick();
        //char buf[64];
        //int_to_string(current_fps, buf);
        //print_string(buf, 0xFFFFFFFF);
        //print_string("\n", 0xFFFFFFFF);
        //swap_buffers();    Do not uncomment this until it's fixed! I mean the loop is not running anyways.
    }


    shell_loop();

    for (;;) {
        asm("hlt");
    }
}
