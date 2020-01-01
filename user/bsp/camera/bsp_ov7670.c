#include "bsp_ov7670.h"
#include "bsp_base.h"
#include "fsl_sccb.h"
#include "fsl_camera.h"
#include "myheap4.h"

#include "fsl_ov7670.h"


void BOARD_InitCSIPins(void);
void BOARD_InitCameraResource(void);

//uint32_t activeFrameAddr;
//uint32_t inactiveFrameAddr;


static csi_private_data_t csiPrivateData;

/* OV7670���ӵ�CSI */
static csi_resource_t csiResource = {
    .csiBase = CSI,
};

camera_receiver_handle_t cameraReceiver = {
    .resource = &csiResource, .ops = &csi_ops, .privateData = &csiPrivateData,
};
/**
  * @brief  ����ͷ�жϴ�����
  * @param  ��
  * @retval ��
  */
void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
	

}
/**
  * @brief  ����ͷ��λ���ſ��ƺ���
  * @param  pullUp��1���ߵ�ƽ��0���͵�ƽ
  * @retval ��
  */
static void BOARD_PullCameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO1, 1, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO1, 1, 0);
    }
}
/**
  * @brief  ����ͷ��Դ���ſ��ƺ���
  * @param  pullUp��1���ߵ�ƽ��0���͵�ƽ
  * @retval ��
  */
static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(GPIO1, 0, 1);
    }
    else
    {
        GPIO_PinWrite(GPIO1, 0, 0);
    }
}
static ov7670_resource_t ov7670Resource = {
    .sccbI2C = LPI2C1,
    .pullResetPin = BOARD_PullCameraResetPin,
    .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
    .inputClockFreq_Hz = 12000000,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov7670Resource, .ops = &ov7670_ops,
};
//4*320*240  64�ֽڶ���
AT_SDRAM1_SECTION_ALIGN(static uint16_t s_frameBuffer[APP_FRAME_BUFFER_COUNT][APP_CAMERA_WIDTH*APP_CAMERA_HEIGHT],
                              FRAME_BUFFER_ALIGN);


 camera_config_t cameraConfig = {
			.pixelFormat = kVIDEO_PixelFormatRGB565,
			.bytesPerPixel = APP_BPP,
			.resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT),
			.frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,
			.interface = kCAMERA_InterfaceGatedClock,
			.controlFlags = APP_CAMERA_CONTROL_FLAGS,
			.framePerSec = 30,
	};
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitCamera
*	����˵��: ��������ͷGPIO��CAMERA�豸.
*	��    ��:  ��
*	�� �� ֵ: ��
AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t s_frameBuffer[APP_FRAME_BUFFER_COUNT][APP_CAMERA_WIDTH][APP_CAMERA_HEIGHT],
                              FRAME_BUFFER_ALIGN);
*********************************************************************************************************
*/
void bsp_InitCamera(void)
{
//	uint16_t *s_frameBuffer;
	/* ��ʼ������ͷ���� */
	BOARD_InitCSIPins();
	/* ��ʼ������ͷ��I2C���������� */
	BOARD_InitCameraResource();

//		s_frameBuffer = staticMalloc(FRAME_BUF_SIZE);
//    memset(s_frameBuffer, 0, FRAME_BUF_SIZE);

    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, (camera_receiver_callback_t)privateCallBack, NULL);

    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    for (uint32_t i = 0; i < APP_FRAME_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_frameBuffer[i]));
    }

    CAMERA_RECEIVER_Start(&cameraReceiver);
}

int camera_GetFullBuffer(uint32_t *activeFrameAddr)
{
		if(kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, activeFrameAddr))//blocking
		{
			return -1;
		}	
		if(*activeFrameAddr<0x80000000)return  -1;
		return 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitCSIPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitCSIPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_00_GPIO1_IO00,        /* CSI_PWR GPIO_AD_B0_04 is configured as GPIO1_IO04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_01_GPIO1_IO01,        /* CSI_RST GPIO_AD_B0_04 is configured as GPIO1_IO04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
#if USE_HARD_I2C
	IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 is configured as LPI2C1_SCL */
      1U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 is configured as LPI2C1_SDA */
      1U);                                    /* Software Input On Field: Input Path is determined by functionality */
#endif
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_12_CSI_PIXCLK,        /* GPIO_AD_B1_04 is configured as CSI_PIXCLK */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
	IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_05_CSI_MCLK,         /* GPIO_AD_B1_05 is configured as csi_mclk */
      0U); 
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_06_CSI_VSYNC,         /* GPIO_AD_B1_06 is configured as CSI_VSYNC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_07_CSI_HSYNC,         /* GPIO_AD_B1_07 is configured as CSI_HSYNC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_08_CSI_DATA09,        /* GPIO_AD_B1_08 is configured as CSI_DATA09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_09_CSI_DATA08,        /* GPIO_AD_B1_09 is configured as CSI_DATA08 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_10_CSI_DATA07,        /* GPIO_AD_B1_10 is configured as CSI_DATA07 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_11_CSI_DATA06,        /* GPIO_AD_B1_11 is configured as CSI_DATA06 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_12_CSI_DATA05,        /* GPIO_AD_B1_12 is configured as CSI_DATA05 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_13_CSI_DATA04,        /* GPIO_AD_B1_13 is configured as CSI_DATA04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_14_CSI_DATA03,        /* GPIO_AD_B1_14 is configured as CSI_DATA03 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_15_CSI_DATA02,        /* GPIO_AD_B1_15 is configured as CSI_DATA02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
#if USE_HARD_I2C			
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 PAD functional properties : */
      0xD8B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 PAD functional properties : */
      0xD8B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
#endif
}


#if USE_HARD_I2C
void BOARD_InitCameraResource(void)
{
    lpi2c_master_config_t masterConfig;
    uint32_t sourceClock;

    LPI2C_MasterGetDefaultConfig(&masterConfig);
		/*����I2Cʱ��Ϊ400KHz*/
    masterConfig.baudRate_Hz = 10000;
    masterConfig.debugEnable = true;
    masterConfig.ignoreAck = true;

    /*LPI2Cʱ������ */
    /*
     * LPI2C ʱ��Դѡ��:
     *  0: pll3_60m
     *  1: OSC clock
     */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, 1);
    /*
     * LPI2C ��Ƶϵ��.
     *  0b000000: Divide by 1
     *  0b111111: Divide by 2^6
     */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);

    /* LPI2C ʱ��ԴΪ OSC */
    sourceClock = CLOCK_GetOscFreq();

    LPI2C_MasterInit(LPI2C1, &masterConfig, sourceClock);

/* CSI MCLK select 24M. */
    /*
     * CSI clock source:
     *
     * 00 derive clock from osc_clk (24M)
     * 01 derive clock from PLL2 PFD2
     * 10 derive clock from pll3_120M
     * 11 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_CsiMux, 0);
    /*
     * CSI clock divider:
     *
     * 000 divide by 1
     * 001 divide by 2
     * 010 divide by 3
     * 011 divide by 4
     * 100 divide by 5
     * 101 divide by 6
     * 110 divide by 7
     * 111 divide by 8
     */
    CLOCK_SetDiv(kCLOCK_CsiDiv, 0);


    /* ��ʼ������ͷ��PDN��RST���� */
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput, 1,
    };

    GPIO_PinInit(GPIO1, 0, &pinConfig);
		GPIO_PinInit(GPIO1, 1, &pinConfig);
}
#else
 
void BOARD_InitCameraResource(void)
{
		//SCL
	  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16, 0U);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16, 0xD8B0u); //
	//SDA
		IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0U);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0xD8B0u); //

  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t pinConfig;      
    
   /** ���İ��LED�ƣ�GPIO���� **/       
  pinConfig.direction = kGPIO_DigitalOutput; //���ģʽ
  pinConfig.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
  pinConfig.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
	GPIO_PinInit(GPIO1, 16, &pinConfig);
	GPIO_PinInit(GPIO1, 17, &pinConfig);
	
	GPIO_PinInit(GPIO1, 0, &pinConfig);
	GPIO_PinInit(GPIO1, 1, &pinConfig);
		
}
#endif

