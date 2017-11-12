#ifndef __SNAKE_H
#define __SNAKE_H

#include "stm32f10x.h"
#include "vga.h"
#include "delay.h"

#define NULL              0x0
#define SNAKE_LEN_MAX     49

#define SNAKE_UP          4
#define SNAKE_RIGHT       5
#define SNAKE_DOWN        6
#define SNAKE_LEFT        7

#define GAME_OVER 1
#define CONTINUE  0

#define FULL      0
#define HUNGRY    1

struct snake_body {
    uint16_t x;
    uint16_t y;
    struct snake_body *next;
};

struct snake {
    uint32_t len;
    uint8_t dir;
};

struct snake_food {
    uint16_t x;
    uint16_t y;
};
    
void draw_block(int16_t x, int16_t y, uint16_t rop);
void draw_snake(void);
void snake_start(void);
int8_t on_snaking(void);
void snake_init(void);
void food_generate(void);
void add_head(int x,int y);
void clear_tail(void);
void is_dir_right(uint8_t dir);
int8_t move_on(void);
void free_snake(void);
void show_score(void);

#endif /* __SNAKE_H */
