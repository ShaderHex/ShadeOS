#include "drivers/display.h"
#include "drivers/keyboard.h"
#include "core/software/shell.h"

int main() {
    clear_screen();
    print_string("Hello OS!\n");
    init_keyboard();
    shell_loop();
    return 0;
}
