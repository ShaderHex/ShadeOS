#pragma once
#include "io.h"

int get_row_from_offset(int offset);
int get_offset(int col, int row);
int move_offset_to_new_line(int offset);
int scroll_ln(int offset);
void set_char_at_video_memory(char character, int offset, unsigned char color);
void set_cursor(int offset);
int get_cursor(void);
void print_string(const char *string, unsigned char color);
void clear_screen();