#include "snake.h"
#include "controller.h"
#include "vga.h"
#include "adcrand.h"
#include "delay.h"

uint16_t move_time = 800;

struct snake_body *tail, *head;
struct snake_food *food;
struct snake *my_snake;
int8_t score_flag = 0;

// Return value: 0 -- alive
//               1 -- dead
void draw_block(int16_t x, int16_t y, uint16_t rop)
{
    uint16_t real_x, real_y;

    // The real x point is 3x-2, and the real y point is 2y-1
    real_x = 3*x-2;
    real_y = (y<<1) - 1;

    // A block contains 3 pixels in x-axis, and 2 pixels in y-axis
    for (int8_t i = 0; i < 3; ++i) {
        for (int8_t j = 0; j < 2; ++j) {
            vga_draw_point(NULL, real_x+i, real_y+i, rop);
        }
    }
}


void draw_snake(void)
{
    struct snake_body *p = tail;

    while (p != NULL) {
        draw_block(p->x, p->y, GDI_ROP_COPY);
        p = p->next;
    }
}


// Main part of the game
void snake_start(void)
{
    vga_clear_screen();
    vga_draw_rec(0, 0, (VID_PIXELS_X - 1), VID_VSIZE - 1, GDI_ROP_COPY);
  	vga_draw_text(185, 70, "SNAKE", GDI_ROP_COPY);
  	vga_draw_text(146, 85, "PRESS START BUTTON", GDI_ROP_COPY);

    // Waiting for the start button to be pressed
    while(getButtonData() != PSB_R2);

    // Draw the border
    vga_clear_screen();
    vga_draw_rec(0, 0, 300, 200, GDI_ROP_COPY);

    // Draw a score notation to the right part of the screen 
    vga_draw_text(332, 70, "SCORE", GDI_ROP_COPY);

    snake_init();
    food_generate();

    draw_snake();
    draw_block(food->x, food->y, GDI_ROP_COPY);
    // Draw the score 
    vga_draw_text(347, 85, "00", GDI_ROP_COPY);

    while(on_snaking() == CONTINUE) {
        draw_snake();
        if (score_flag == FULL) {
            food_generate();
            draw_block(food->x, food->y, GDI_ROP_COPY);
            vga_draw_text(332, 85, u32_2pu8(my_snake->len - 6), GDI_ROP_COPY);
        }
        delay_ms(move_time);
    }

    // When game over
    free_snake();
    while(1)
        show_score();
}


int8_t on_snaking(void)
{
    is_dir_right(getButtonData());
    return move_on();
}


void snake_init(void)
{
    int i;

    tail = head = (struct snake_body*)malloc(sizeof(struct snake_body));
    tail->x = 7;
    tail->y = 50;
    tail->next = NULL;

    food = (struct snake_food*)malloc(sizeof(struct snake_food));
    my_snake = (struct snake*)malloc(sizeof(struct snake));
    my_snake->dir = SNAKE_RIGHT;
    my_snake->len = 6;
    for(i = 0; i < my_snake->len - 1; i++)
        add_head(tail->x + 1, tail->y);
}


void food_generate(void)
{
    // Using "m" to mark whether the food is generrating correctly
    int m = 1;
    struct snake_body *p = tail;
    srand(adc_ran_num());
    food->x = rand() % SNAKE_LEN_MAX + 1;
    food->y = rand() % SNAKE_LEN_MAX + 1;
    while(p != NULL) {
        if(p->x==food->x && p->y==food->y) {
            m = 0;
            break;
        }
        p = p->next;
    }

    // Re-generate food
    if (m == 0) food_generate();
}


void add_head(int x,int y)
{
    struct snake_body *newnode = (struct snake_body*)malloc(sizeof(struct snake_body));
    newnode->x = x;
    newnode->y = y;
    newnode->next = NULL;
    head->next = newnode;
    head = newnode;
}


void clear_tail(void)
{
    struct snake_body *trash;
    trash = tail;
    tail = tail->next;

    // Clear the area and free the buffer
    draw_block(trash->x, trash->y, GDI_ROP_XOR);
    free(trash);
}


void is_dir_right(uint8_t dir)
{
    if ((int8_t)my_snake->dir - (int8_t)dir == -2 || (int8_t)my_snake->dir - (int8_t)dir == 2)
        return;
    else
        my_snake->dir = dir;
}


int8_t move_on(void)
{
    struct snake_body *ph;
    struct snake_body *pt;
    ph = head;
    switch (my_snake->dir) {
    case SNAKE_UP:
        add_head(ph->x,ph->y-1); break;
    case SNAKE_DOWN:
        add_head(ph->x,ph->y+1); break;
    case SNAKE_LEFT:
        add_head(ph->x-1,ph->y); break;
    case SNAKE_RIGHT:
        add_head(ph->x+1,ph->y); break;
    }

    // See if the snake is eating food
    if(head->x == food->x && head->y == food->y) {
        my_snake->len++;
        score_flag = FULL;
    }
    if(score_flag == HUNGRY)
        clear_tail();

    // See if the snake is eating itself (ewwwwwww)
    ph = head;
    pt = tail;
    while(pt != head) {
        if(ph->x == pt->x && ph->y == pt->y)
            return GAME_OVER;
        pt = pt->next;
    }

    // Decide whether the snake is crushing the border
    if (head->x<1 || head->x>SNAKE_LEN_MAX || head->y<1 || head->y>SNAKE_LEN_MAX)
        return GAME_OVER;

    return CONTINUE;
}

void free_snake(void)
{
    struct snake_body *p = tail, *tmp;
    while (p != NULL) {
        tmp = p;
        p = p->next;
        free(tmp);
    }
}


void show_score(void)
{
    vga_clear_screen();
    vga_draw_rec(0,0,(VID_PIXELS_X - 1),VID_VSIZE - 1,0);
	  vga_draw_text(175, 55, "SNAKE", GDI_ROP_COPY);
	  vga_draw_text(146, 100, "SURPRISE, UR MOTHERFUKER", GDI_ROP_COPY);
}
