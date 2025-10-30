#include "../../drivers/display.h"
#include "shell.h"

void shell_loop(void) {
    clear_screen();
    print_string("Welcome to the shell!\n");
}
