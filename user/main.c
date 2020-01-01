/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��V2.3.1�汾�⽨�Ĺ���ģ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "main.h"
#include "t_debug.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"  
#include "app_dhcp.h"
#include "myheap4.h"
/*******************************************************************
 * Prototypes 
 *******************************************************************/

extern void freeRtosInit(void);
_SYS	Sys;
/*******************************************************************
 * Code
 *******************************************************************/

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	  
    /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
    /* ��ʼ������������ */
  //  BOARD_InitPins();
    /* ��ʼ��������ʱ�� */
    BOARD_BootClockRUN();
    /* ��ʼ�����Դ��� */
   // BOARD_InitDebugConsole();
		/*��ʼ�����Դ��� ����1��RX�����ź���*/
		T_Debug_Init();
	
    /* ��ӡϵͳʱ�� */
    Ac_log("\r\n");
    Ac_log("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
    Ac_log("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    Ac_log("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    Ac_log("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    Ac_log("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    Ac_log("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    Ac_log("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    Ac_log("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    Ac_log("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
    /* �����������Ĵ���^_^. */
  
		
 //   LED_GPIO_Config(); //CSIռ��
		bsp_enet_config();
		freeRtosInit();
    vTaskStartScheduler();
		
    while(1)
    {
      
    }     

}


void __aeabi_assert(const char *str,const char *file,int line)
{
	Ac_log("Wrong %s: file %s on line %d\r\n",str,file,line);
	while(1)
	__BKPT(0);
}
/****************************END OF FILE**********************/
