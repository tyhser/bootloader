#include "kservice.h"
#include "drv_gpio.h"
#include "drv_uart.h"
#include "download.h"

#define LED 0

void delay_us(volatile bl_uint16_t us)
{
	bl_uint32_t n = 1000*us;
	while (n--){}
}

int main(void)
{
	bl_uint32_t kernel_addr = 0x000000ff;
	bl_uint32_t kernel_len = 0;
	bl_uint32_t image_addr = 0x00004000;
	bl_uint32_t image_len = 0;

	drv_gpio_init((void *)0);
	drv_uart_init((void *)0);

	bl_kprintf("enter main\n");

	image_len = download_image((bl_uint32_t *)image_addr);
	kernel_len = download_kernel((bl_uint32_t *)kernel_addr);

	if (image_len != 0 && kernel_len != 0) {
		((void (*)(void *img, void * res0, void * res1, void * res2))kernel_addr)(image_addr, 0, 0, 0);
	}

	while (1) {
		bl_kprintf("boot fail!\n");
		drv_gpio_write(LED, 1);
		delay_us(1000);
		drv_gpio_write(LED, 0);
	}
}
