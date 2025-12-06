#include "drivers/display.h"
#include "core/software/shell.h"
#include "core/PIT.h"
#include "core/idt.h"
#include "core/gdt.h"
#include "core/TSC.h"
#include "drivers/panic.h"
#include "drivers/hwinfo.h"
#include "core/scheduler.h"
#include "core/task.h"
#include <limine.h>


void delay(int count) {
    for (volatile int i = 0; i < count; i++);
}

void task() {
    print_string("Task 1\n", 0xFFFFFFFF);
}

void task2() {
    print_string("Task 2\n", 0xFFFFFFFF);
}

void task3() {
    print_string("Task 3\n", 0xFFFFFFFF);
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
    int a = 1/0;

    asm("sti");

    print_string("\nShadeOS x86_64 Pre-release!\n", 0xFFFFFFFF);
    

    scheduler_add(task);
    scheduler_add(task2);
    scheduler_add(task3);

    scheduler_run(); /* Comment this line if you'd like to use the shell */

    shell_loop();

    for (;;) {
        asm("hlt");
    }
}
