#include "stdint.h"
#include "Uart.h"

volatile PL011_t *Uart = (PL011_t *)UART_BASE_ADDRESS0;