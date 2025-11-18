#pragma once
#include "../../../../drivers/display.h"
#include "../../../../drivers/keyboard.h"
#include "../../../../drivers/io.h"
#include "../../../../core/lib/string.h"

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

    static struct snake snake_body[MAX_SNAKE] = {{5,5},{4,5},{3,5}};
    int snake_length = 3;
    enum Direction dir = RIGHT;

    struct snake food = { rand_range(0, WIDTH-3), rand_range(0, HEIGHT-3) };
    char buf[16];

    while(isRunning) {
        clear_screen();

        // Non-blocking input
        if (port_byte_in(0x64) & 0x01) {
            unsigned char scancode = keyboard_read_scancode();
            if(scancode != 0) {
                char ch = keyboard_scancode_to_char(scancode);
                if (ch == 'w' && dir != DOWN) dir = UP;
                else if (ch == 's' && dir != UP) dir = DOWN;
                else if (ch == 'a' && dir != RIGHT) dir = LEFT;
                else if (ch == 'd' && dir != LEFT) dir = RIGHT;
            }
        }

        // Move snake body
        for(int i = snake_length - 1; i > 0; i--)
            snake_body[i] = snake_body[i-1];

        // Move head
        switch(dir) {
            case UP: snake_body[0].y--; break;
            case DOWN: snake_body[0].y++; break;
            case LEFT: snake_body[0].x--; break;
            case RIGHT: snake_body[0].x++; break;
        }

        // Check wall collisions
        if (snake_body[0].x < 0 || snake_body[0].x >= WIDTH ||
            snake_body[0].y < 0 || snake_body[0].y >= HEIGHT) {
            isRunning = 0;
        }

        // Check self collisions
        for(int i = 1; i < snake_length; i++)
            if(snake_body[0].x == snake_body[i].x && snake_body[0].y == snake_body[i].y)
                isRunning = 0;

        // Eat food
        if(snake_body[0].x == food.x && snake_body[0].y == food.y) {
            if(snake_length < MAX_SNAKE) snake_length++;
            do {
                food.x = rand_range(0, WIDTH-1);
                food.y = rand_range(0, HEIGHT-1);
            } while (0); // TODO: avoid spawning on snake
        }

        // Draw top border
        for(int x = 0; x < WIDTH+2; x++) print_string("*", 0xFFFFFFFF);
        print_string("\n", 0xFFFFFFFF);

        // Draw field
        for(int y = 0; y < HEIGHT; y++) {
            print_string("*", 0xFFFFFFFF); // left border
            for(int x = 0; x < WIDTH; x++) {
                int drawn = 0;
                for(int i = 0; i < snake_length; i++)
                    if(snake_body[i].x == x && snake_body[i].y == y) {
                        print_string("O", 0xFFFFFFFF);
                        drawn = 1;
                        break;
                    }
                if(!drawn && food.x == x && food.y == y) {
                    print_string("F", 0xFFFFFFFF);
                    drawn = 1;
                }
                if(!drawn) print_string(" ", 0xFFFFFFFF);
            }
            print_string("*\n", 0xFFFFFFFF); // right border
        }

        // Bottom border
        for(int x = 0; x < WIDTH+2; x++) print_string("*", 0xFFFFFFFF);
        print_string("\n", 0xFFFFFFFF);

        // Memory usage
        int_to_string(snake_length * sizeof(struct snake), buf);
        print_string("Memory in use: ", 0xFFFFFFFF);
        print_string(buf, 0xFFFFFFFF);
        print_string("\n", 0xFFFFFFFF);

        // Simple delay
        for(volatile int delay = 0; delay < 10000000; delay++);
    }

    print_string("\nGame Over!\n", 0xFFFFFFFF);
}
