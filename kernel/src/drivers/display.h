#pragma once
#include <stdint.h>
#include "io.h"
#include <limine.h>
#include <stdbool.h>
#include <stddef.h>

void init_framebuffer();
int get_row_from_offset(int offset);
int get_offset(int col, int row);
int move_offset_to_new_line(int offset);
int scroll_ln(int offset);
void set_char_at_video_memory(struct limine_framebuffer *fb, int x, int y, char c, uint32_t color);
void set_cursor(int offset);
int get_cursor(void);
void print_string(int x, int y, const char *string, uint32_t color);
void print_hex(uint32_t val);
void clear_screen();