#include "bsp_enet.h"
#include "bsp_base.h"
static void BOARD_InitEnetPins(void) { 

   CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 is configured as GPIO1_IO09 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 is configured as GPIO1_IO10 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 is configured as ENET_RX_ER */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 is configured as ENET_RX_DATA00 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 is configured as ENET_RX_DATA01 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 is configured as ENET_RX_EN */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 is configured as ENET_TX_DATA00 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 is configured as ENET_TX_DATA01 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 is configured as ENET_TX_EN */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 is configured as ENET_REF_CLK */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_B1_10 */

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_04_ENET_MDC,            /* GPIO_EMC_40 is configured as ENET_MDC */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_15_ENET_MDIO,           /* GPIO_EMC_41 is configured as ENET_MDIO */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 PAD functional properties : */
        0xB0A9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 PAD functional properties : */
        0xB0A9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
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

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_04_ENET_RX_DATA00,       /* GPIO_B1_04 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_05_ENET_RX_DATA01,       /* GPIO_B1_05 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/5
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Pull
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                   Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_06_ENET_RX_EN,           /* GPIO_B1_06 PAD functional properties : */
        0xB0E9u);
                                                /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/5
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Pull
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                   Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_07_ENET_TX_DATA00,       /* GPIO_B1_07 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                    Drive Strength Field: R0/5
                                                    Speed Field: max(200MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Pull
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Up

                                                 Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_08_ENET_TX_DATA01,       /* GPIO_B1_08 PAD functional properties : */
        0xB0E9u);                              /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/5
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Pull
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                   Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_09_ENET_TX_EN,           /* GPIO_B1_09 PAD functional properties : */
        0xB0E9u);                              /* Slew Rate Field: Fast Slew Rate
                                                    Drive Strength Field: R0/5
                                                    Speed Field: max(200MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Pull
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                    Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_10_ENET_REF_CLK,         /* GPIO_B1_10 PAD functional properties : */
        0x31u);                                /* Slew Rate Field: Fast Slew Rate
                                                    Drive Strength Field: R0/6
                                                    Speed Field: low(50MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Disabled
                                                    Pull / Keep Select Field: Keeper
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                    Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_11_ENET_RX_ER,           /* GPIO_B1_11 PAD functional properties : */
        0xB0E9u);                               /* Slew Rate Field: Fast Slew Rate
                                                    Drive Strength Field: R0/5
                                                    Speed Field: max(200MHz)
                                                    Open Drain Enable Field: Open Drain Disabled
                                                    Pull / Keep Enable Field: Pull/Keeper Enabled
                                                    Pull / Keep Select Field: Pull
                                                    Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                    Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B1_04_ENET_MDC,
        0xB0E9u);                              /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/5
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Pull
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                   Hyst. Enable Field: Hysteresis Disabled */

    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_15_ENET_MDIO,
        0xB829u);                              /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/5
                                                   Speed Field: low(50MHz)
                                                   Open Drain Enable Field: Open Drain Enabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Pull
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                   Hyst. Enable Field: Hysteresis Disabled */
			
}




static void _enet_phy_reset_by_gpio(void)
{
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    bsp_delay_ms(50);
    GPIO_WritePinOutput(GPIO1, 9, 1);

}

static void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, false, 1};
    CLOCK_InitEnetPll(&config);
}

void bsp_enet_config()
{
		BOARD_InitEnetPins();
	  BOARD_InitModuleClock();
		IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
		_enet_phy_reset_by_gpio();
}
