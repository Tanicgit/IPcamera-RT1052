#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "t_debug.h"

#include "bsp_lpuart.h"
#include "fsl_lpuart.h"

//SemaphoreHandle_t myMutexLogHandle;
SemaphoreHandle_t myCountingSemLogHandle;
#define RX_BUFFER_SIZE	64
static uint8_t PAfifo[RX_BUFFER_SIZE];
static uint32_t tail=0;
static uint32_t head=RX_BUFFER_SIZE-1;
extern __IO uint32_t uwTick;

int8_t in(uint8_t a)
{
	uint32_t p;
	p=tail;
	if(p==head)return -1;
	else
	{
		PAfifo[p] = a;
		tail = p<RX_BUFFER_SIZE-1 ? p+1 : 0;
		return 0;
	}
}


int8_t Fifoout(uint8_t *a)
{
	uint32_t p;
	p= head;
	p= p<RX_BUFFER_SIZE-1 ? p+1:0;
	if(p==tail)
	{
		return -1;
	}
	else
	{
		*a = PAfifo[p];
		head = p;
		return 0;
	}
}

void readCharCallback(uint8_t a)
{
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	in(a);
	xSemaphoreGiveFromISR( myCountingSemLogHandle, &xHigherPriorityTaskWoken );
}

int LOG_ReadOneCharacter(uint8_t *ch)
{
		uint8_t t=0;
    /* wait release from ISR */
    if( xSemaphoreTake( myCountingSemLogHandle, portMAX_DELAY ) == pdTRUE )
		{
			if(0==Fifoout(&t))
			*ch = t;
			return 0;
		} 
		else
		{
			return -1;
		}
}



int LOG_ReadLine(uint8_t *buf,uint8_t size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        /* recieve one char every time */
        if (LOG_ReadOneCharacter(&buf[i]) != 0)
        {
            return -1;
        }
        /* analysis data */
        if ((buf[i] == '\r') || (buf[i] == '\n'))
        {
            /* End of Line. */
            if (i == 0)
            {
                buf[i] = '\0';
                i = -1;
            }
            else
            {
                break;
            }
        }
    }
    /* get char should not add '\0'*/
    if (i == size)
    {
        buf[i] = '\0';
    }
    else
    {
        buf[i + 1] = '\0';
    }


    return i;
}
#include <stdarg.h>
#include <stdlib.h>
#include "t_str.h"
int DbgConsole_Scanf(char *fmt_ptr, ...)
{
    va_list ap;
    int result;
    char scanfBuf[TSHELL_SCANF_MAX_LOG_LEN + 1U] = {0U};
    LOG_ReadLine((uint8_t *)scanfBuf, TSHELL_SCANF_MAX_LOG_LEN);
    va_start(ap, fmt_ptr);
    result = StrFormatScanf(scanfBuf, fmt_ptr, ap);
    va_end(ap);
    return result;
}
static void DbgConsole_RelocateLog(char *buf, int32_t *indicator, char val, int len)
{
    int i = 0;

    for (i = 0; i < len; i++)
    {
        if ((*indicator + 1) >= TSHELL_PRINTF_MAX_LOG_LEN)
        {
					LPUART_WriteBlocking(LPUART1,(uint8_t*)buf,*indicator);
            *indicator = 0U;
        }
        buf[*indicator] = val;
        (*indicator)++;
    }
}
int DbgConsole_Printf(const char *fmt_s, ...)
{
    va_list ap;
    int logLength = 0U, result = 0U;
    char printBuf[TSHELL_PRINTF_MAX_LOG_LEN] = {0U};
    va_start(ap, fmt_s);
    logLength = StrFormatPrintf(fmt_s, ap, printBuf, DbgConsole_RelocateLog);
		LPUART_WriteBlocking(LPUART1,(uint8_t*)printBuf,logLength);
    va_end(ap);
    return result;
}

void t_send_data(uint8_t *data,uint32_t len)
{
//	HAL_UART_Transmit(&huart3,(uint8_t*)data,len,0xffff);
	LPUART_WriteBlocking(LPUART1,(uint8_t*)data,len);
}
void t_recv_data(uint8_t *data,uint32_t len)
{
	uint32_t i=0;
	if(len==1)
	{
		if(LOG_ReadOneCharacter(data)<0)
			*data=(uint8_t)-1;
	}
	else{
		for(i=0;i<len;i++)
		{
			LOG_ReadOneCharacter(data+i);
		}
	}
}


//static StaticSemaphore_t xSemaphoreBuffer;
#include "bsp_lpuart.h"
void T_Debug_Init()
{
//	myMutexLogHandle     = xSemaphoreCreateMutex();
	myCountingSemLogHandle = xSemaphoreCreateCounting( TSHELL_SCANF_MAX_LOG_LEN+1, 0);
	
	LPUART1_Rx_Callback = readCharCallback;
	LPUART1_Config(115200);
}





