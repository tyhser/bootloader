#ifndef _DRV_DMA_H
#define _DRV_DMA_H

#include "drv_config.h"

int drv_dma_init(void *hdma);
int drv_dma_deinit(void *hdma);

int drv_dma_start(void *hdma, UINT32 src, UINT32 dst, UINT32 data_length);
int drv_dma_abort(void *hdma);

#endif
