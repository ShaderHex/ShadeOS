#pragma once
#include <stdint.h>

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
void int_to_string(unsigned int n, char* buf);
void io_wait(void);
void uint_to_hex(uint64_t val, char* buf);
void uint_to_dec(uint64_t val, char* buf);