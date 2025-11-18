#pragma once
#include <limine.h>
#include "display.h"
#include "io.h"

extern volatile struct limine_memmap_request memmap_request;
void print_memmap();
void print_memory_info_detailed();
void print_memory_info();