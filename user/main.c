#include "stm32f10x.h"
#include "snake.h"
#include "vga.h"
#include "delay.h"
#include "adcrand.h"
#include "controller.h"

void rcc_config(void) 
{
    /* TIM1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  | RCC_APB2Periph_SPI1 | RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

int main(void)
{
    delay_init(72);
    
    rcc_config();
    vga_init();
    controller_config();
    
    snake_start();
    
    return 0;
}


