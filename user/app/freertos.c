#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "t_debug.h"
#include "bsp.h" 
#include "GUI.h"

#include "t_debug.h"
#include "t_shell.h"

#include	"lwip_app.h"
#include "main.h"
#include "GUI_VNC.h"
#include "app_ui.h"
#include "bsp_ov7670.h"



#define	EN_VNC	0  //内部ram模拟显存
#define EN_TCP_SERVER 1
#define	EN_LWIP	(EN_VNC|EN_TCP_SERVER)

#if (EN_VNC+EN_TCP_SERVER>1)
#error "EN_LCD+EN_VNC+EN_IPC>1"
#endif

#define	EN_LCD  1


extern __IO	uint32_t stream_cnt;

BaseType_t xReturnedLogLater;
TaskHandle_t xHandleLogLater = NULL;
void vTaskLogLater( void * pvParameters )
{
	for( ;; )
	{  
		vTaskDelay(10000);
	//	RGB_RED_LED_TOGGLE;
		
	//	Ac_log("FPS = %d\r\n",stream_cnt/10);
		stream_cnt=0;
		#if EN_LWIP
		Sys.dhcpSta=CheckDHCPsta();
		#endif
	}
}
BaseType_t xReturnedShell;
TaskHandle_t xHandleShell = NULL;
shell_context_struct t_shell;
extern const shell_command_context_t xDhcpCommand;
extern const shell_command_context_t xIfconfigCommand;
extern const shell_command_context_t xNetifCommand;
extern const shell_command_context_t xSocketCommand ;
extern const shell_command_context_t xIpCommand ;
void vTaskCodeShell( void * pvParameters )
{
	SHELL_Init(&t_shell,t_send_data,t_recv_data,DbgConsole_Printf,"T#->");
	SHELL_RegisterCommand(&xDhcpCommand);
	SHELL_RegisterCommand(&xIfconfigCommand);
	SHELL_RegisterCommand(&xNetifCommand);
	SHELL_RegisterCommand(&xSocketCommand);
	SHELL_RegisterCommand(&xIpCommand);
	for( ;; )
	{  
		SHELL_Main(&t_shell);
	}
}
extern GUI_CONST_STORAGE GUI_BITMAP bm1;
extern GUI_CONST_STORAGE GUI_BITMAP bm2;
BaseType_t xReturnedUi;
TaskHandle_t xHandleUi = NULL;
void vTaskCodeUi(void * pvParameters)
{

	#if (EN_LWIP)
	while(Sys.dhcpSta==0)
	{
		osDelay(200);
	}
	#endif
	
	#if (EN_VNC)
	  int i=0;
		GUI_Init();
		GUI_VNC_X_StartServer(0,0);
		GUI_VNC_SetPassword((uint8_t*)"123456");
		GUI_VNC_SetProgName("tanic");
		GUI_VNC_SetSize(V_LCD_W,V_LCD_H);
		GUI_VNC_RingBell();
		
		GUI_SetBkColor(GUI_BLUE);
		GUI_SetColor(GUI_WHITE);
		GUI_Clear();	
	  for(;;)
		{
			GUI_Delay(1000);
			GUI_DispDecAt(i++,100,100,5);
		}	
	#else
		#if (EN_TCP_SERVER)
			xReturnedTcpServer = xTaskCreate(task_tcp_server, "task_tcp_server",1024, ( void * ) 1,4,&xHandleTcpServer );
		#endif

		#if (EN_LCD)
			task_creamra_jpeg_lcd_stream();
		#else
			task_creamra_jpeg_stream();
		#endif
	#endif
}

void freeRtosInit()
{
	#if EN_LWIP
		LWIP_Init();//必须调度开始前初始化 原因不详
	#endif
	  xReturnedLogLater = xTaskCreate(vTaskLogLater, "vTaskLogLater",128, ( void * ) 1,3,&xHandleLogLater ); 
		xReturnedShell = xTaskCreate(vTaskCodeShell, "vTaskCodeShell",2048, ( void * ) 1,9,&xHandleShell ); 
		xReturnedUi = xTaskCreate(vTaskCodeUi, "vTaskCodeUi",2048, ( void * ) 1,5,&xHandleUi );
		
}
