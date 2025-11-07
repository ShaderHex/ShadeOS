#include "drivers/display.h"
#include "drivers/keyboard.h"
#include "drivers/panic.h"
#include "core/software/shell.h"

int main() {
    clear_screen();
    print_string("Hello OS!\n", 0x0f);
    init_keyboard();
    keyboard_enable();
    //kpanic("Test");
    shell_loop();
    return 0;
}
