#include "../../drivers/display.h"
#include "../../drivers/keyboard.h"
#include "../../drivers/io.h"
#include "../../core/lib/string.c"
#include "shell/command/help.h"
#include "shell/command/snake.h"
#include "shell/command/systeminfo.h"
#include "shell.h"


void execute_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "shutdown") == 0) {
        print_string("\nSystem shutting down...\n");
    } else if (strcmp(cmd, "snake") == 0) {
        cmd_snake();
    } else if (strcmp(cmd, "sysinfo") == 0) {
        cmd_systeminfo();
    } else{
        print_string("\nUnknown command: ");
        print_string((char*)cmd);
        print_string("\n");
    }
}

void shell_loop(void) {
    //clear_screen();
    print_string("Welcome to the shell!\n> ");

    char command[128];
    int index = 0;

    while (1) {
        unsigned char status = port_byte_in(0x64);
        if (status & 0x01) {
            unsigned char scancode = keyboard_read_scancode();
            char ch = keyboard_scancode_to_char(scancode);

            if (scancode == 0x1C) {
                command[index] = '\0';
                execute_command(command);
                index = 0;
                print_string("> ");
            }
            else if (scancode == 0x0E) {
                if (index > 0) {
                    index--;
                    print_string("\b \b");
                }
            }
            else if (ch != 0 && index < 127) {
                command[index++] = ch;
                char str[2] = { ch, 0 };
                print_string(str);
            }
        }
    }
}
