#ifndef __APP_UI_H
#define __APP_UI_H
#include "fsl_common.h"
#include "lwip_app.h" 


void task_creamra_jpeg_stream(void);
void task_creamra_jpeg_lcd_stream(void);

extern BaseType_t xReturnedTcpServer;
extern  TaskHandle_t xHandleTcpServer ;
void task_tcp_server(void * pvParameters);

unsigned long app_jpeg_stream_out(uint8_t **data);
#endif
