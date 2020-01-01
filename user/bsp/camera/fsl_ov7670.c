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
	����ΪOV7670 QVGA RGB565����  (by guanfu_wang)  http://mcudiy.taobao.com

	����RA8875ͼ��ģʽ�£�ɨ�跽��Ϊ�����ң����ϵ��¡�
	��wang_guanfu�ṩ��ȱʡֵ��ͬ����������ʵ��ĵ�����
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
//{0x03, 0x06},//0x0a,???��?����??��????

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
	{0x1e, 0x07},//0x07, 0x17, 0x27, 0x37 ѡ��1��������ɨ�跽��. ��Ҫ��LCD��ɨ�跽��ƥ�䡣
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
	{0x55, 0x00},//����
	{0x56, 0x40},//�Աȶ�
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
    /* ��ʱ */
    OV7670_DelayMs(1);

    resource->pullPowerDownPin(false);

    /* ��ʱ */
    OV7670_DelayMs(2);

    resource->pullResetPin(true);

    /* ��ʱ */
    OV7670_DelayMs(2);
    /* ʶ���豸 */
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
    /* �豸ʶ��ȷ��, ִ�������λ */
    status = OV7670_WriteReg(i2c,0x12, 0x80); 	/* Reset SCCB */
		if (kStatus_Success != status)
    {
        return status;
    }
		
    /* ��ʱ */
    OV7670_DelayMs(2);

    /* ��������ͷ�Ĵ��� */
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
/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define	I2C_SCL_GPIO	GPIO1
#define	I2C_SDA_GPIO	GPIO1
#define I2C_SCL_PIN		16			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		17			/* ���ӵ�SDA�����ߵ�GPIO */

	/* �����дSCL��SDA�ĺ� */
#define I2C_SCL_1()  (I2C_SCL_GPIO->DR|=(1u<<I2C_SCL_PIN))				/* SCL = 1 */
#define I2C_SCL_0()  (I2C_SCL_GPIO->DR&=~(1u<<I2C_SCL_PIN))				/* SCL = 0 */

#define I2C_SDA_1()  (I2C_SDA_GPIO->DR|=(1u<<I2C_SDA_PIN))				/* SDA = 1 */
#define I2C_SDA_0()  (I2C_SDA_GPIO->DR&=~(1u<<I2C_SDA_PIN))				/* SDA = 0 */

#define I2C_SDA_IN()	(I2C_SDA_GPIO->GDIR &=~(1u<<I2C_SDA_PIN))
#define I2C_SDA_OUT()	(I2C_SDA_GPIO->GDIR |=(1u<<I2C_SDA_PIN))

//#define I2C_SDA_READ()  ((I2C_SDA_GPIO->PSR&(1u<<I2C_SDA_PIN))!=0)	/* ��SDA����״̬ */
//#define I2C_SCL_READ()  ((I2C_SCL_GPIO->PSR&(1u<<I2C_SCL_PIN))!=0)	/* ��SCL����״̬ */

#define I2C_SDA_READ()  ((I2C_SDA_GPIO->DR&(1u<<I2C_SDA_PIN))!=0)	/* ��SDA����״̬ */
#define I2C_SCL_READ()  ((I2C_SCL_GPIO->DR&(1u<<I2C_SCL_PIN))!=0)	/* ��SCL����״̬ */
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */
#define OV7670_SLAVE_ADDRESS	0x42
/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
		CPU��Ƶ168MHzʱ�����ڲ�Flash����, MDK���̲��Ż�����̨ʽʾ�����۲Ⲩ�Ρ�
		ѭ������Ϊ5ʱ��SCLƵ�� = 1.78MHz (����ʱ: 92ms, ��д������������ʾ����̽ͷ���ϾͶ�дʧ�ܡ�ʱ��ӽ��ٽ�)
		ѭ������Ϊ10ʱ��SCLƵ�� = 1.1MHz (����ʱ: 138ms, ���ٶ�: 118724B/s)
		ѭ������Ϊ30ʱ��SCLƵ�� = 440KHz�� SCL�ߵ�ƽʱ��1.0us��SCL�͵�ƽʱ��1.2us

		��������ѡ��2.2Kŷʱ��SCL������ʱ��Լ0.5us�����ѡ4.7Kŷ����������Լ1us

		ʵ��Ӧ��ѡ��400KHz���ҵ����ʼ���
	*/
	for (i = 0; i < 150; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_OUT();
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    ��:  _ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    ��:  ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    ��:  ��
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
//	I2C_SDA_IN();
	
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	
	I2C_SDA_IN();
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_OUT();
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_OUT();
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    ��:  _Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	if (I2C_SDA_READ() && I2C_SCL_READ())
	{
		i2c_Start();		/* ���������ź� */

		/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
		i2c_SendByte(_Address | I2C_WR);
		ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

		i2c_Stop();			/* ����ֹͣ�ź� */

		return ucAck;
	}
	return 1;	/* I2C�����쳣 */
}


/*
*********************************************************************************************************
*	�� �� ��: OV_WriteReg
*	����˵��: д0V7670�Ĵ���
*	��    ��: _ucRegAddr  : �Ĵ�����ַ
*			  _ucRegValue : �Ĵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
status_t OV7670_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue)
{

    i2c_Start();							/* ���߿�ʼ�ź� */

    i2c_SendByte(OV7670_SLAVE_ADDRESS);		/* �����豸��ַ+д�ź� */
	i2c_WaitAck();

    i2c_SendByte(_ucRegAddr);				/* ���ͼĴ�����ַ */
	i2c_WaitAck();

    i2c_SendByte(_ucRegValue);				/* ���ͼĴ�����ֵ */
	i2c_WaitAck();

    i2c_Stop();                   			/* ����ֹͣ�ź� */
	return kStatus_Success;
}

/*
*********************************************************************************************************
*	�� �� ��: OV_ReadReg
*	����˵��: ��0V7670�Ĵ���
*	��    ��: _ucRegAddr  : �Ĵ�����ַ
*	�� �� ֵ: �Ĵ���ֵ
*********************************************************************************************************
*/
status_t OV7670_ReadReg(uint8_t _ucRegAddr,uint8_t *value)
{
	uint16_t usRegValue;

	i2c_Start();                  			/* ���߿�ʼ�ź� */
	i2c_SendByte(OV7670_SLAVE_ADDRESS);		/* �����豸��ַ+д�ź� */
	i2c_WaitAck();
	i2c_SendByte(_ucRegAddr);				/* ���͵�ַ */
	i2c_WaitAck();

	i2c_Stop();			/* 0V7670 ��Ҫ���� stop, �����ȡ�Ĵ���ʧ�� */

	i2c_Start();                  			/* ���߿�ʼ�ź� */
	i2c_SendByte(OV7670_SLAVE_ADDRESS + 1);/* �����豸��ַ+���ź� */
	i2c_WaitAck();

	usRegValue = i2c_ReadByte();       		/* �������ֽ����� */
	i2c_NAck();
	i2c_Stop();                  			/* ����ֹͣ�ź� */
	*value = usRegValue;
	return kStatus_Success;
}
#endif
