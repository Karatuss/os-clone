#ifndef HAL_HALTIMER_H_
#define HAL_HALTIMER_H_

#include "stdint.h"

void Hal_timer_init(void);
uint32_t Hal_timer_get_1ms_counter(void);

#endif  /* HAL_HALTIMER_H_  */
