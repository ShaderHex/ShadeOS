#pragma once
#include <stdint.h>
#include "../../drivers/panic.h"

extern uint8_t _end;
static uintptr_t heap_ptr;

int strcmp(const char *s1, const char *s2);
int strlen(const char *str);
void strcpy(char *dest, const char *src);
void kmemcpy(void *dst, const void *src, size_t n);
void heap_init();
void* kmalloc(size_t size);
void strcat(char *dest, const char *src);