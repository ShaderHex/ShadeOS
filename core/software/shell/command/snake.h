#pragma once
#include "../../../../drivers/display.h"
#include "../../../../drivers/keyboard.h"
#include "../../../../drivers/io.h"

#define WIDTH 25
#define HEIGHT 15
#define MAX_SNAKE 20

struct snake {
    int x, y;
};

static unsigned int seed = 1234;
unsigned int rand() {
    seed = (1103515245 * seed + 12345) & 0x7fffffff;
    return seed;
}
unsigned int rand_range(unsigned int min, unsigned int max) {
    return min + (rand() % (max - min + 1));
}

enum Direction { UP, DOWN, LEFT, RIGHT };

void cmd_snake() {

    int isRunning = 1;

    struct snake snake_body[MAX_SNAKE] = {{5,5},{4,5},{3,5}};
    int snake_length = 3;

    enum Direction dir = RIGHT;

    struct snake food = { rand_range(0, WIDTH-3), rand_range(0, HEIGHT-3) };

    while(isRunning == 1) {

        clear_screen();

        // Read input (non-blocking)
        if (port_byte_in(0x64) & 0x01) {
            unsigned char scancode = keyboard_read_scancode();
            char ch = keyboard_scancode_to_char(scancode);

            // Change direction based on input
            if (ch == 'w' && dir != DOWN) dir = UP;
            else if (ch == 's' && dir != UP) dir = DOWN;
            else if (ch == 'a' && dir != RIGHT) dir = LEFT;
            else if (ch == 'd' && dir != LEFT) dir = RIGHT;
        }

        // Move body each segment follows previous
        for(int i = snake_length - 1; i > 0; i--) {
            snake_body[i] = snake_body[i-1];
        }

        // Move head
        switch(dir) {
            case UP:    snake_body[0].y--; break;
            case DOWN:  snake_body[0].y++; break;
            case LEFT:  snake_body[0].x--; break;
            case RIGHT: snake_body[0].x++; break;
        }

        // Check collisions with walls
        if (snake_body[0].x < 0 || snake_body[0].x >= WIDTH-2 ||
            snake_body[0].y < 0 || snake_body[0].y >= HEIGHT-2) {
            isRunning = 0;
        }

        // Check collisions with self
        for(int i = 1; i < snake_length; i++) {
            if(snake_body[0].x == snake_body[i].x && snake_body[0].y == snake_body[i].y) {
                isRunning = 0;
            }
        }

        // Check if head eats food
        if(snake_body[0].x == food.x && snake_body[0].y == food.y) {
            if(snake_length < MAX_SNAKE) snake_length++;
            food.x = rand_range(0, WIDTH-3);
            food.y = rand_range(0, HEIGHT-3);
        }

        // This draw the top border
        for(int x = 0; x < WIDTH; x++) print_string("*", 0x0f);
        print_string("\n", 0x0f);

        // This draw the height of the border
        for(int y = 0; y < HEIGHT - 2; y++) {
            print_string("*", 0x0f);
            for(int x = 0; x < WIDTH - 2; x++) {
                int printed = 0;

                // Draw snake segments
                for(int i = 0; i < snake_length; i++) {
                    if(snake_body[i].x == x && snake_body[i].y == y) {
                        print_string("O", 0x0f);
                        printed = 1;
                        break;
                    }
                }

                // Draw food
                if(!printed && food.x == x && food.y == y) {
                    print_string("F", 0x0f);
                    printed = 1;
                }

                if(!printed) print_string(" ", 0x0f);
            }
            // Right border with new line (smart move by me)
            print_string("*\n", 0x0f);
        }

        // Draw bottom border
        for(int x = 0; x < WIDTH; x++) print_string("*", 0x0f);
        print_string("\n", 0x0f);

        for(volatile int delay = 0; delay < 50000000; delay++);
    }

    print_string("\nGame Over!\n", 0x0f);
}
