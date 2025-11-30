#include <stddef.h>
#include "string.h"


#define KERNEL_BASE   0xffffffff80000000UL
#define KERNEL_END ((uintptr_t)&_end)

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strlen(const char *str) {
    int len = 0;
    while (str[len])
        len++;
    return len;
}

void strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

static inline int is_valid_address(const void *addr, size_t size) {
    uintptr_t a = (uintptr_t)addr;
    if (a < KERNEL_BASE || a + size > KERNEL_END) {
        return 0; // invalid
    }
    return 1; // valid
}

void kmemcpy(void *dst, const void *src, size_t n) {
    if (!is_valid_address(dst, n) || !is_valid_address(src, n)) {
        kpanic_memcpy((void*)src, dst, n);
    }
    unsigned char *d = (unsigned char*)dst;
    const unsigned char *s = (const unsigned char*)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

}


void heap_init() {
    heap_ptr = (uintptr_t)&_end;
}


void* kmalloc(size_t size) {
    void* ptr = (void*)heap_ptr;
    heap_ptr += size;

    if (heap_ptr & 7)
        heap_ptr = (heap_ptr + 7) & ~7;

    return ptr;
}

void strcat(char *dest, const char *src) {
    while (*dest) dest++;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}