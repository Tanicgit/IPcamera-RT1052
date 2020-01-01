#include "bsp_base.h"
#include "FreeRTOS.h"
#include "timers.h"


void bsp_delay_ms(uint32_t ms)
{
	uint32_t cnt=0;
	cnt = ms*(SystemCoreClock/2000);
	while(cnt--);
}

void osDelay(uint32_t ms)
{
	vTaskDelay(ms);
}




