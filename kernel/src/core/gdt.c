#include "gdt.h"

void create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    descriptor  =  limit       & 0x000F0000;
    descriptor |= (flag <<  8) & 0x00F0FF00;
    descriptor |= (base >> 16) & 0x000000FF;
    descriptor |=  base        & 0xFF000000;
 
    descriptor <<= 32;
 
    descriptor |= base  << 16;
    descriptor |= limit  & 0x0000FFFF;
    
    print_string("GDT created descriptor: ", 0xFFFFFFFF);
    print_hex(descriptor);
    print_string("\n", 0xFFFFFFFF);
}
 

void gdt_init() {
    create_descriptor(0, 0, 0);
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
}