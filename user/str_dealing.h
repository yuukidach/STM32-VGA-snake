#ifndef __STR_DEALING
#define __STR_DEALING

#include "stm32f10x.h"

typedef	uint8_t             *pu8;

uint16_t strLen(pu8 str);
pu8 u32_2pu8(uint32_t num);

#endif  /* __STR_DEALING */

