#include "irq.h"

irq_handler_t irq_handlers[16];

void set_irq_handler(int irq, irq_handler_t fn) {
    irq_handlers[irq] = fn;
}

extern void pic_eoi();

void irq_dispatch(uint64_t irq) {
    print_string("irq_dispatch got called", 0xFFFFFFFF);
    if (irq < 16 && irq_handlers[irq])
        irq_handlers[irq]();
    pic_eoi();
}

void irq_common_handler(uint64_t* stack) {
    print_string("irq_common_handler got called", 0xFFFFFFFF);
    uint64_t int_no = stack[1];
    irq_dispatch(int_no - 32);
}