#include "io.h"

unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

void int_to_string(unsigned int n, char* buf) {
    int i = 0;
    if (n == 0) {
        buf[i++] = '0';
        buf[i] = 0;
        return;
    }
    while (n) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    buf[i] = 0;

    for (int j = 0; j < i / 2; j++) {
        char t = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = t;
    }
}

void io_wait(void) {
    port_byte_out(0x80, 0);
}