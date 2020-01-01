#include "bsp_lpuart.h"
#include "bsp_base.h"
#include "fsl_lpuart.h"
static void LPUART1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
}


void LPUART1_Config(uint32_t baudRate)
{
	uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();
	
	lpuart_config_t lpuart_config;
	
	LPUART1_InitPins();
	
	LPUART_GetDefaultConfig(&lpuart_config);
	lpuart_config.baudRate_Bps = baudRate;
	/* Enable clock and initial UART module follow user configure structure. */
	LPUART_Init(LPUART1, &lpuart_config, uartClkSrcFreq);
	LPUART_EnableTx(LPUART1, true);
	LPUART_EnableRx(LPUART1, true);
	
	LPUART_EnableInterrupts(LPUART1, kLPUART_RxDataRegFullInterruptEnable);
	NVIC_SetPriority(LPUART1_IRQn,5);
	EnableIRQ(LPUART1_IRQn);
	
	
}
void (*LPUART1_Rx_Callback)(uint8_t)=NULL;
void LPUART1_IRQHandler(void)
{
    uint8_t data;
    if ((kLPUART_RxDataRegFullFlag) & LPUART_GetStatusFlags(LPUART1))
    {
        data = LPUART_ReadByte(LPUART1);
      //  LPUART_WriteByte(LPUART1, data);
			if(LPUART1_Rx_Callback!=NULL)LPUART1_Rx_Callback(data);
    }  
		#if defined __CORTEX_M && (__CORTEX_M == 4U)		
    __DSB();
		#endif
}

