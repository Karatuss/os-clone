#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "HalTimer.h"

void delay(uint32_t ms)
{
    uint32_t goal = Hal_timer_get_1ms_counter() + ms;

    while (goal != Hal_timer_get_1ms_counter());
}

void memclr(void *dst, uint32_t count)
{
    uint8_t *d = (uint8_t*)dst;

    while (count--)
        *d++ = 0;
}