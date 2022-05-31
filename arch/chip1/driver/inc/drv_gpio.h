#ifndef _DRV_GPIO_H
#define _DRV_GPIO_H
#include "drv_config.h"

int drv_gpio_init(void *io);
int drv_gpio_write(void *io, BOOL state);
int drv_gpio_read(void *io);

#endif
