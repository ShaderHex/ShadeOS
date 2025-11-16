#pragma once

#include "io.h"
#include "display.h"

void keyboard_enable();
void keyboard_disable();
void init_keyboard();
unsigned char keyboard_read_scancode();
char keyboard_scancode_to_char(unsigned char scancode);