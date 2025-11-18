#pragma once
#include "../../../../drivers/display.h"

void cmd_help() {
    print_string("\nAvailable commands:\nhelp\nclear\nshutdown\n", 0xFFFFFFFF);
}