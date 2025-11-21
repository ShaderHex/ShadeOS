#pragma once
#include <stdint.h>

typedef void (*irq_handler_t) ();
extern irq_handler_t irq_handlers[16];

void set_irq_handler(int irq, irq_handler_t fn);