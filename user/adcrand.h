#ifndef __ADCRAND_H
#define __ADCRAND_h
#include "stm32f10x.h"
#include <stdlib.h>

void adc1_c2_config(void);
void dma1_c1_config(void);
void adcrand_init(void);
uint16_t adc_ran_num(void);

#endif /* __ADC_RAND_H */




