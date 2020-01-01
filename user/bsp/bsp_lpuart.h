#ifndef __BSP_LPUART_H
#define __BSP_LPUART_H

#include "fsl_common.h"


extern void (*LPUART1_Rx_Callback)(uint8_t);
void LPUART1_Config(uint32_t baudRate);


#endif /* __BSP_LED_H */
