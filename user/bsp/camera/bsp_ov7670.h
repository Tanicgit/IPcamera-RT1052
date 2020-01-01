#ifndef __BSP_OV7670_H
#define __BSP_OV7670_H
	#include "fsl_common.h"
	#include "fsl_sccb.h"
	#include "fsl_csi.h"
	#include "fsl_camera.h"
	
	/* Camera definition. */
	#define APP_CAMERA_HEIGHT 240
	#define APP_CAMERA_WIDTH 320
	#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

	/* Frame buffer data alignment. */
	#define FRAME_BUFFER_ALIGN 64


	#define APP_FRAME_BUFFER_COUNT 4
	
	#define FRAME_BUF_SIZE	SDK_SIZEALIGN(APP_CAMERA_WIDTH*APP_CAMERA_HEIGHT*APP_BPP*APP_FRAME_BUFFER_COUNT, FRAME_BUFFER_ALIGN)
	/* Pixel format RGB565, bytesPerPixel is 2. */
	#define APP_BPP 2

	#define FRAME_RATE_30FPS	0 //30֡
	#define FRAME_RATE_15FPS	1 //15֡
	
	
	#define APP_BPP 2

	extern camera_config_t cameraConfig;

	int camera_GetFullBuffer(uint32_t *activeFrameAddr);		
	void bsp_InitCamera(void);
#endif
