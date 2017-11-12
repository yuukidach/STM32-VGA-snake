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

