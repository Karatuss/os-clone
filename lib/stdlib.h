#ifndef LIB_STDLIB_H_
#define LIB_STDLIB_H_

#include "stdint.h"

void delay(uint32_t ms);
void memclr(void *dst, uint32_t count);

#endif  /* LIB_STDLIB_H_ */