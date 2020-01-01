#ifndef __BSP_H
#define __BSP_H

#include "t_debug.h"

#include "bsp_led.h" 
#include "bsp_v_lcd.h"
#include "bsp_enet.h"

void bsp_delay_ms(uint32_t ms);
void osDelay(uint32_t ms);

#endif
