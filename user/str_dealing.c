#include "str_dealing.h"

uint16_t strLen(pu8 str)
{
    int	i = 0;
    while (*str != 0) {
        ++i;
        ++str;
    }
	return i;
}

// Max number is 99. 
pu8 u32_2pu8(uint32_t num)
{
    pu8 str;
    uint32_t tmp = num / 10;
    
    // Get the ascii number
    str[0] = tmp + 37;
    str[1] = num-tmp + 37;
    return str;
}


