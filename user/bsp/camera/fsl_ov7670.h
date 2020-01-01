#ifndef __OV7670_H
#define	__OV7670_H

#include "fsl_common.h"
#include "fsl_sccb.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_lpi2c.h"
#include "fsl_camera_device.h"
#include "fsl_csi_camera_adapter.h"

#define USE_HARD_I2C	1

#if USE_HARD_I2C
#define OV7670_SCCB_ADDR 0x21U
#define OV7670_WriteReg(i2c,reg, val) SCCB_WriteReg(i2c, OV7670_SCCB_ADDR, kSCCB_RegAddr8Bit, (reg), (val))

#define OV7670_ReadReg(i2c,reg, val) SCCB_ReadReg(i2c, OV7670_SCCB_ADDR, kSCCB_RegAddr8Bit, (reg), (val))
#else
status_t OV7670_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue);
status_t OV7670_ReadReg(uint8_t _ucRegAddr,uint8_t *value);
#endif
#define OV7670_PID_REG 0x0AU        /*!< Product ID Number MSB */
#define OV7670_VER_REG 0x0BU        /*!< Product ID Number LSB */
typedef struct _ov7670_reg
{
    uint16_t reg;
    uint8_t val;
} ov7670_reg_t;
/*!
 * @brief OV7670 resource.
 *
 * Before initialize the OV5640, the resource must be initialized that the SCCB
 * I2C could start to work.
 */
typedef struct _ov7670_resource
{
    sccb_i2c_t sccbI2C;                    /*!< I2C for SCCB transfer. */
    void (*pullResetPin)(bool pullUp);     /*!< Function to pull reset pin high or low. */
    void (*pullPowerDownPin)(bool pullUp); /*!< Function to pull the power down pin high or low. */
    uint32_t inputClockFreq_Hz;            /*!< Input clock frequency. */
} ov7670_resource_t;

extern const camera_device_operations_t ov7670_ops;
extern void CSI_DriverIRQHandler(void);

#endif
