#include "display.h"
#include <stdint.h>
#define FONT_SCALE 2

#define PANIC_BG   0xFF0000
#define PANIC_FG   0xFFFFFF

static int center_text_x(const char* text, int screen_width) {
    int text_width = strlen(text) * 8 * FONT_SCALE;
    return (screen_width - text_width) / 2;
}

void kpanic(const char* msg) {
    uint32_t screen_width  = framebuffer->width;
    uint32_t screen_height = framebuffer->height;

    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            putpixel(framebuffer, x, y, PANIC_BG);
        }
    }

    int title_x = center_text_x("!! KERNEL PANIC !!", screen_width);
    int title_y = screen_height / 4;
    print_string_xy(title_x, title_y, "!! KERNEL PANIC !!", PANIC_FG);

    int reason_label_x = center_text_x("Panic Reason: ", screen_width);
    int reason_y = title_y + 32;
    print_string_xy(reason_label_x, reason_y, "Panic Reason: ", PANIC_FG);

    int reason_x = center_text_x(msg, screen_width);
    print_string_xy(reason_x, reason_y + 16, msg, PANIC_FG);

    for (;;) {
        __asm__("hlt");
    }
}
