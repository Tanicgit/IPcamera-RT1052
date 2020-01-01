#ifndef __BSP_BASE_H
#define __BSP_BASE_H

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "pad_config.h" 
#include "t_debug.h"
#include "pin_mux.h"
#include "board.h"
#include "t_debug.h"

void bsp_delay_ms(uint32_t ms);
void osDelay(uint32_t ms);
#endif
