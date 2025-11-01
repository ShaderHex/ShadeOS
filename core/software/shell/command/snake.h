#pragma once
#include "../../../../drivers/display.h"

#define WIDTH 25
#define HEIGHT 15

struct snake {
    int x, y;
};

void cmd_snake() {

    int isRunning = 1;

    // Example snake with 3 segments
    struct snake snake_body[3] = {{5,5},{4,5},{3,5}};
    int snake_length = 3;

    while(isRunning == 1) {
        clear_screen();

        // This draw the top border
        for (int x = 0; x < WIDTH; x++) {
            print_string("*");
        }
        print_string("\n");
    
        // This draw the height of the border.
        // We have to subtract 2 from the height since the upper loop will draw it
        for (int y = 0; y < HEIGHT - 2; y++) {
            print_string("*");  // left border
            for (int x = 0; x < WIDTH - 2; x++) {
                int printed = 0;

                // Check if snake occupies this cell
                for (int i = 0; i < snake_length; i++) {
                    if (snake_body[i].x == x && snake_body[i].y == y) {
                        print_string("O");  // snake body
                        printed = 1;
                        break;
                    }
                }

                if (!printed) {
                    print_string(" ");
                }
            }
            // Right border with new line (smart move by me)
            print_string("*\n");
        }
    
        // Draw bottom border
        for (int x = 0; x < WIDTH; x++) {
            print_string("*");
        }
        print_string("\n");

        // Stop after one loop for now (Im going to remove this after implementing movement)
        isRunning = 0;
    }
}
