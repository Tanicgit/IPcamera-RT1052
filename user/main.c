/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   用V2.3.1版本库建的工程模板
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
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
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	  
    /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
    /* 初始化开发板引脚 */
  //  BOARD_InitPins();
    /* 初始化开发板时钟 */
    BOARD_BootClockRUN();
    /* 初始化调试串口 */
   // BOARD_InitDebugConsole();
		/*初始化调试串口 创建1个RX计数信号量*/
		T_Debug_Init();
	
    /* 打印系统时钟 */
    Ac_log("\r\n");
    Ac_log("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
    Ac_log("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    Ac_log("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    Ac_log("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    Ac_log("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    Ac_log("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    Ac_log("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    Ac_log("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    Ac_log("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
    /* 在这里添加你的代码^_^. */
  
		
 //   LED_GPIO_Config(); //CSI占用
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
