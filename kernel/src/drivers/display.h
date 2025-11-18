#pragma once
#include <stdint.h>
#include "io.h"
#include <limine.h>
#include <stdbool.h>
#include <stddef.h>
#include "../core/lib/string.h"

extern struct limine_framebuffer *framebuffer;

void init_framebuffer();
int get_row_from_offset(int offset);
int get_offset(int col, int row);
int move_offset_to_new_line(int offset);
int scroll_ln(int lines);
void set_char_at_video_memory(struct limine_framebuffer *fb, int x, int y, char c, uint32_t color, int scale);
void set_cursor(int offset);
int get_cursor(void);
void print_string(const char *string, uint32_t color);
void print_string_xy(int x, int y, const char *string, uint32_t color);
void print_hex(uint64_t val);
void clear_screen();
void putpixel(struct limine_framebuffer *fb, int x, int y, uint32_t color);