#include "fsl_ov7670.h"
#include "bsp.h"
static void OV7670_DelayMs(uint32_t ms)
{
    volatile uint32_t i;
    uint32_t loopPerMs = SystemCoreClock / 3000;

    while (ms--)
    {
        i = loopPerMs;
        while (i--)
        {
        }
    }
}





/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t OV7670_Init(camera_device_handle_t *handle, const camera_config_t *config);

status_t OV7670_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);

status_t OV7670_Start(camera_device_handle_t *handle);

status_t OV7670_Stop(camera_device_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t ov7670_ops = {
    .init = OV7670_Init,
    .start = OV7670_Start,
    .stop = OV7670_Stop,
    .init_ext = OV7670_InitExt,
};
/*
	以下为OV7670 QVGA RGB565参数  (by guanfu_wang)  http://mcudiy.taobao.com

	由于RA8875图形模式下，扫描方向为从左到右，从上到下。
	和wang_guanfu提供的缺省值不同。因此做了适当的调整。
*/

//static const ov7670_reg_t ov7670InitRegs[]={
//{0x3a, 0x04},//dummy
//{0x40, 0xd0},//565   
//{0x12, 0x14},//QVGA,RGB

//{0x32, 0x80},//HREF control	bit[2:0] HREF start 3 LSB	bit[5:3] HSTOP HREF end 3LSB
//{0x17, 0x16},//HSTART start high 8-bit MSB         
//{0x18, 0x04},//5 HSTOP end high 8-bit
//{0x19, 0x02},
//{0x1a, 0x7b},//0x7a,
//{0x03, 0x06},//0x0a,???ú?±·??ò????

//{0x0c, 0x00},
//{0x15, 0x00},//0x00
//{0x3e, 0x00},//10
//{0x70, 0x3a},
//{0x71, 0x35},
//{0x72, 0x11},
//{0x73, 0x00},//
//        
//{0xa2, 0x02},//15
//{0x11, 0x81},//
//{0x7a, 0x20},
//{0x7b, 0x1c},
//{0x7c, 0x28},
//        
//{0x7d, 0x3c},//20
//{0x7e, 0x55},
//{0x7f, 0x68},
//{0x80, 0x76},
//{0x81, 0x80},
//        
//{0x82, 0x88},
//{0x83, 0x8f},
//{0x84, 0x96},
//{0x85, 0xa3},
//{0x86, 0xaf},
//        
//{0x87, 0xc4},//30
//{0x88, 0xd7},
//{0x89, 0xe8},
//{0x13, 0xe0},
//{0x00, 0x00},//AGC
//        
//{0x10, 0x00},
//{0x0d, 0x00},//
//{0x14, 0x28},//0x38, limit the max gain
//{0xa5, 0x05},
//{0xab, 0x07},
//        
//{0x24, 0x75},//40
//{0x25, 0x63},
//{0x26, 0xA5},
//{0x9f, 0x78},
//{0xa0, 0x68},
//        
//{0xa1, 0x03},//0x0b,
//{0xa6, 0xdf},//0xd8,
//{0xa7, 0xdf},//0xd8,
//{0xa8, 0xf0},
//{0xa9, 0x90},
//        
//{0xaa, 0x94},//50
//{0x13, 0xe5},
//{0x0e, 0x61},
//{0x0f, 0x4b},
//{0x16, 0x02},
//        
//{0x1e, 0x37},//
//{0x21, 0x02},
//{0x22, 0x91},
//{0x29, 0x07},
//{0x33, 0x0b},
//        
//{0x35, 0x0b},//60
//{0x37, 0x1d},
//{0x38, 0x71},
//{0x39, 0x2a},
//{0x3c, 0x78},
//        
//{0x4d, 0x40},
//{0x4e, 0x20},
//{0x69, 0x00},
//{0x6b, 0x40},//PLL*4=48Mhz
//{0x74, 0x19},
//{0x8d, 0x4f},
//        
//{0x8e, 0x00},//70
//{0x8f, 0x00},
//{0x90, 0x00},
//{0x91, 0x00},
//{0x92, 0x00},//0x19,//0x66
//        
//{0x96, 0x00},
//{0x9a, 0x80},
//{0xb0, 0x84},
//{0xb1, 0x0c},
//{0xb2, 0x0e},
//        
//{0xb3, 0x82},//80
//{0xb8, 0x0a},
//{0x43, 0x14},
//{0x44, 0xf0},
//{0x45, 0x34},
//        
//{0x46, 0x58},
//{0x47, 0x28},
//{0x48, 0x3a},
//{0x59, 0x88},
//{0x5a, 0x88},
//        
//{0x5b, 0x44},//90
//{0x5c, 0x67},
//{0x5d, 0x49},
//{0x5e, 0x0e},
//{0x64, 0x04},
//{0x65, 0x20},
//        
//{0x66, 0x05},
//{0x94, 0x04},
//{0x95, 0x08},
//{0x6c, 0x0a},
//{0x6d, 0x55},
//        
//        
//{0x4f, 0x80},
//{0x50, 0x80},
//{0x51, 0x00},
//{0x52, 0x22},
//{0x53, 0x5e},
//{0x54, 0x80},
//        
////{0x54, 0x40},//110
//        
//        
//{0x09, 0x03},//

//{0x6e, 0x11},//100
//{0x6f, 0x9f},//0x9e for advance AWB
//  {0x55, 0x00},//
//  {0x56, 0x40},//
//  {0x57, 0x40},//0x40,  change according to Jim's request 

//{0x6a, 0x40},
//{0x01, 0x40},
//{0x02, 0x40},
//{0x13, 0xe7},
//{0x15, 0x00},  


//{0x58, 0x9e},

//{0x41, 0x08},
//{0x3f, 0x00},
//{0x75, 0x05},
//{0x76, 0xe1},
//{0x4c, 0x00},
//{0x77, 0x01},
//{0x3d, 0xc2}, 
//{0x4b, 0x09},
//{0xc9, 0x60},
//{0x41, 0x38},

//{0x34, 0x11},
//{0x3b, 0x02}, 

//{0xa4, 0x89},
//{0x96, 0x00},
//{0x97, 0x30},
//{0x98, 0x20},
//{0x99, 0x30},
//{0x9a, 0x84},
//{0x9b, 0x29},
//{0x9c, 0x03},
//{0x9d, 0x4c},
//{0x9e, 0x3f},
//{0x78, 0x04},

//{0x79, 0x01},
//{0xc8, 0xf0},
//{0x79, 0x0f},
//{0xc8, 0x00},
//{0x79, 0x10},
//{0xc8, 0x7e},
//{0x79, 0x0a},
//{0xc8, 0x80},
//{0x79, 0x0b},
//{0xc8, 0x01},
//{0x79, 0x0c},
//{0xc8, 0x0f},
//{0x79, 0x0d},
//{0xc8, 0x20},
//{0x79, 0x09},
//{0xc8, 0x80},
//{0x79, 0x02},
//{0xc8, 0xc0},
//{0x79, 0x03},
//{0xc8, 0x40},
//{0x79, 0x05},
//{0xc8, 0x30},
//{0x79, 0x26}, 
//{0x09, 0x00}
//};

static const ov7670_reg_t  ov7670InitRegs[] =
{
	
	
	{0x3a, 0x0c},
	{0x67, 0xc0},
	{0x68, 0x80},

	{0x40, 0xd0},
	{0x12, 0x14}, //COM7  0X14 320*240 RBG      0x11

	{0x32, 0x80},
	{0x17, 0x16},
	{0x18, 0x04},
	{0x19, 0x02},
	{0x1a, 0x7a},//0x7a,

	{0x03, 0x05},//0x0a,
	{0x0c, 0x00},
	{0x3e, 0x00},//

	{0x70, 0x00},
	{0x71, 0x01},
	{0x72, 0x11},
	{0x73, 0x00},//
	{0xa2, 0x02},
	{0x11, 0x01},//  /2

	{0x7a,  0x2C},
	{0x7b,  0x11},
	{0x7c,  0x1a},
	{0x7d,  0x2a},
	{0x7e,  0x42},
	{0x7f,  0x4c},
	{0x80,  0x56},
	{0x81,  0x5f},
	{0x82,  0x67},
	{0x83,  0x70},
	{0x84,  0x78},
	{0x85,  0x87},
	{0x86,  0x95},
	{0x87,  0xaf},
	{0x88,  0xc8},
	{0x89,  0xdf},

	////////////////
	{0x13, 0xe0},
	{0x00, 0x00},//AGC

	{0x10, 0x00},
	{0x0d, 0x00},
	{0x14, 0x10},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},

	{0x24, 0x75},//40
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},

	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},

	{0xaa, 0x94},//50
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},

#if 1
	{0x1e, 0x07},//0x07, 0x17, 0x27, 0x37 选择1个，决定扫描方向. 需要和LCD的扫描方向匹配。
#else
	{0x1e, 0x27},//0x07,
#endif


	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},

	{0x35, 0x0b},//60
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},

	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x5d},
	{0x6b, 0x40},//PLL  X4
	{0x74, 0x19},
	{0x8d, 0x4f},

	{0x8e, 0x00},//70
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66

	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},

	{0xb3, 0x82},//80
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},

	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},

	{0x5b, 0x44},//90
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},

	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},


	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},

	{0x76, 0xe1},

	{0x6e, 0x11},//100
	{0x6f, 0x9f},//0x9e for advance AWB
	{0x55, 0x00},//亮度
	{0x56, 0x40},//对比度
	{0x57, 0x80},//0x40,
                            

};

static status_t OV7670_WriteRegs(sccb_i2c_t i2c,const ov7670_reg_t regs[], uint32_t num)
{
    status_t status = kStatus_Success;

    for (uint32_t i = 0; i < num; i++)
    {	
        status = OV7670_WriteReg(i2c,regs[i].reg, regs[i].val);

        if (kStatus_Success != status)
        {
            break;
        }
    }

    return status;
}
status_t OV7670_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    status_t status;
    uint8_t pid=0, ver=0;
    uint16_t width=0, height=0;
    ov7670_resource_t *resource = (ov7670_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;

    if ((kCAMERA_InterfaceNonGatedClock != config->interface) && (kCAMERA_InterfaceGatedClock != config->interface) &&
        (kCAMERA_InterfaceCCIR656 != config->interface))
    {
        return kStatus_InvalidArgument;
    }

    width = FSL_VIDEO_EXTRACT_WIDTH(config->resolution);
    height = FSL_VIDEO_EXTRACT_HEIGHT(config->resolution);

    if ((width > 320) || (height > 240))
    {
        return kStatus_InvalidArgument;
    }

    resource->pullPowerDownPin(true);
		resource->pullResetPin(false);
    /* 延时 */
    OV7670_DelayMs(1);

    resource->pullPowerDownPin(false);

    /* 延时 */
    OV7670_DelayMs(2);

    resource->pullResetPin(true);

    /* 延时 */
    OV7670_DelayMs(2);
    /* 识别设备 */
    status = OV7670_ReadReg(i2c,0x0a, &pid);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = OV7670_ReadReg(i2c,0x0b, &ver);
    if (kStatus_Success != status)
    {
        return status;
    }

    if (0x7673 != (((uint32_t)pid << 8U) | (uint32_t)ver))
    {			
			
    }
			Ac_log("OV7670ID=%04X\r\n",((uint32_t)pid << 8U) | (uint32_t)ver);
    /* 设备识别确定, 执行软件复位 */
    status = OV7670_WriteReg(i2c,0x12, 0x80); 	/* Reset SCCB */
		if (kStatus_Success != status)
    {
        return status;
    }
		
    /* 延时 */
    OV7670_DelayMs(2);

    /* 配置摄像头寄存器 */
    status = OV7670_WriteRegs(i2c,ov7670InitRegs, sizeof(ov7670InitRegs)/sizeof(ov7670InitRegs[0]));
    if (kStatus_Success != status)
    {
        return status;
    }
		return kStatus_Success;
}

status_t OV7670_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t OV7670_Stop(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t OV7670_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return OV7670_Init(handle, config);
}



#if !USE_HARD_I2C
/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define	I2C_SCL_GPIO	GPIO1
#define	I2C_SDA_GPIO	GPIO1
#define I2C_SCL_PIN		16			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		17			/* 连接到SDA数据线的GPIO */

	/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_1()  (I2C_SCL_GPIO->DR|=(1u<<I2C_SCL_PIN))				/* SCL = 1 */
#define I2C_SCL_0()  (I2C_SCL_GPIO->DR&=~(1u<<I2C_SCL_PIN))				/* SCL = 0 */

#define I2C_SDA_1()  (I2C_SDA_GPIO->DR|=(1u<<I2C_SDA_PIN))				/* SDA = 1 */
#define I2C_SDA_0()  (I2C_SDA_GPIO->DR&=~(1u<<I2C_SDA_PIN))				/* SDA = 0 */

#define I2C_SDA_IN()	(I2C_SDA_GPIO->GDIR &=~(1u<<I2C_SDA_PIN))
#define I2C_SDA_OUT()	(I2C_SDA_GPIO->GDIR |=(1u<<I2C_SDA_PIN))

//#define I2C_SDA_READ()  ((I2C_SDA_GPIO->PSR&(1u<<I2C_SDA_PIN))!=0)	/* 读SDA口线状态 */
//#define I2C_SCL_READ()  ((I2C_SCL_GPIO->PSR&(1u<<I2C_SCL_PIN))!=0)	/* 读SCL口线状态 */

#define I2C_SDA_READ()  ((I2C_SDA_GPIO->DR&(1u<<I2C_SDA_PIN))!=0)	/* 读SDA口线状态 */
#define I2C_SCL_READ()  ((I2C_SCL_GPIO->DR&(1u<<I2C_SCL_PIN))!=0)	/* 读SCL口线状态 */
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */
#define OV7670_SLAVE_ADDRESS	0x42
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 150; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_OUT();
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_OUT();
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	I2C_SDA_OUT();
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	I2C_SDA_IN();
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
//	I2C_SDA_IN();
	
	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	
	I2C_SDA_IN();
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_OUT();
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_OUT();
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参:  _Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	if (I2C_SDA_READ() && I2C_SCL_READ())
	{
		i2c_Start();		/* 发送启动信号 */

		/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
		i2c_SendByte(_Address | I2C_WR);
		ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

		i2c_Stop();			/* 发送停止信号 */

		return ucAck;
	}
	return 1;	/* I2C总线异常 */
}


/*
*********************************************************************************************************
*	函 数 名: OV_WriteReg
*	功能说明: 写0V7670寄存器
*	形    参: _ucRegAddr  : 寄存器地址
*			  _ucRegValue : 寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
status_t OV7670_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue)
{

    i2c_Start();							/* 总线开始信号 */

    i2c_SendByte(OV7670_SLAVE_ADDRESS);		/* 发送设备地址+写信号 */
	i2c_WaitAck();

    i2c_SendByte(_ucRegAddr);				/* 发送寄存器地址 */
	i2c_WaitAck();

    i2c_SendByte(_ucRegValue);				/* 发送寄存器数值 */
	i2c_WaitAck();

    i2c_Stop();                   			/* 总线停止信号 */
	return kStatus_Success;
}

/*
*********************************************************************************************************
*	函 数 名: OV_ReadReg
*	功能说明: 读0V7670寄存器
*	形    参: _ucRegAddr  : 寄存器地址
*	返 回 值: 寄存器值
*********************************************************************************************************
*/
status_t OV7670_ReadReg(uint8_t _ucRegAddr,uint8_t *value)
{
	uint16_t usRegValue;

	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(OV7670_SLAVE_ADDRESS);		/* 发送设备地址+写信号 */
	i2c_WaitAck();
	i2c_SendByte(_ucRegAddr);				/* 发送地址 */
	i2c_WaitAck();

	i2c_Stop();			/* 0V7670 需要插入 stop, 否则读取寄存器失败 */

	i2c_Start();                  			/* 总线开始信号 */
	i2c_SendByte(OV7670_SLAVE_ADDRESS + 1);/* 发送设备地址+读信号 */
	i2c_WaitAck();

	usRegValue = i2c_ReadByte();       		/* 读出高字节数据 */
	i2c_NAck();
	i2c_Stop();                  			/* 总线停止信号 */
	*value = usRegValue;
	return kStatus_Success;
}
#endif
