#ifndef _DRV_UART_H
#define _DRV_UART_H
#include "drv_config.h"

int drv_uart_transmit(void *huart, UINT8 *pData, UINT16 Size, UINT32 Timeout);
int drv_uart_receive(void *huart, UINT8 *pData, UINT16 Size, UINT32 Timeout);

#endif
