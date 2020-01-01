#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "t_debug.h"
#include "bsp.h" 
#include "GUI.h"

#include "t_debug.h"
#include "t_shell.h"
#include "main.h"


#include "lwip/dhcp.h"
#include "lwip/api.h"
#include "lwip/opt.h"
#include "lwip_app.h" 

#include "myheap4.h"

#define  X_SIZE		320
#define	 Y_SIZE		240


GUI_BITMAP bm0 ;
//typedef struct {
//  U16P XSize;
//  U16P YSize;
//  U16P BytesPerLine;
//  U16P BitsPerPixel;
//  const U8 * pData;
//  const GUI_LOGPALETTE * pPal;
//  const GUI_BITMAP_METHODS * pMethods;
//} GUI_BITMAP;
//void * creatbmp(int xsize,int ysize,int bpp)
//{
//	void *p;
//	p = staticMalloc(xsize*ysize*bpp);
//	if(p==NULL)return NULL ;

//	bm0.XSize = xsize;
//	bm0.YSize = ysize;
//	bm0.BytesPerLine = xsize*bpp;
//	bm0.BitsPerPixel = bpp;
//	bm0.pData = p;
//	bm0.pPal = NULL;
//	bm0.pMethods = GUI_DRAW_BMPM565;
//	return p;
//}

#include "fsl_elcdif.h"
#include "jinclude.h"
#include "jpeglib.h"

#include "bsp_ov7670.h"
#include "fsl_ov7670.h"
#include "bsp_lcd.h"
int rgb888_jpeg_compress(unsigned char *inbuf, unsigned char **outbuf,unsigned long *outSize);
extern GUI_CONST_STORAGE GUI_BITMAP bm1;
extern camera_receiver_handle_t cameraReceiver;



SemaphoreHandle_t xJpegstreamSemaphore = NULL;

struct _jpeg{
	uint8_t *jpeg;
	unsigned long size;
};

#define FIFO_NUM	4
static struct _jpeg JPEG[FIFO_NUM];
static uint32_t tail=0;
static uint32_t head=FIFO_NUM-1;

static uint32_t jpeg_stream_init()
{
	uint32_t i=0;
	for(i=0;i<FIFO_NUM;i++)
	{
		JPEG[i].jpeg = staticMalloc(SDK_SIZEALIGN(X_SIZE*Y_SIZE*3,FRAME_BUFFER_ALIGN));
		if(JPEG[i].jpeg==0)
		{
			return 0;
		}
	}
	return i;
}


//
static int driver_jpeg_stream_in()
{
	uint32_t p;
	p=tail;
	if(p==head)return -1;
	else
	{	
		tail = p<FIFO_NUM-1 ? p+1 : 0;	
		return p;
	}
}


unsigned long app_jpeg_stream_out(uint8_t **data)
{
	uint32_t p;
	p= head;
	p= p<FIFO_NUM-1 ? p+1:0;
	if(p==tail)
	{
		return 0;
	}
	else
	{
		*data = JPEG[p].jpeg;
		head = p;
		xSemaphoreGive( xJpegstreamSemaphore );
		return JPEG[p].size;
	}
}

uint8_t *rbg888=0;
unsigned long rgb565_jpeg(uint16_t *rgb565,uint8_t **jpeg)
{	
	
	uint32_t j=0;	
	unsigned long jpeg_size=0;
	
  if(rbg888==0)rbg888 = staticMalloc(SDK_SIZEALIGN(X_SIZE*Y_SIZE*3,FRAME_BUFFER_ALIGN));
	jpeg_size = SDK_SIZEALIGN(X_SIZE*Y_SIZE*3,FRAME_BUFFER_ALIGN);
	
		for(int i=0;i<X_SIZE*Y_SIZE;i++)
		{
			rbg888[j++] = ((rgb565[i]&0xf800)>>11)<<3;
			rbg888[j++] = ((rgb565[i]&0x7e0)>>5)<<2;
			rbg888[j++] = (rgb565[i]&0x1f)<<3;
		}		
		rgb888_jpeg_compress(rbg888,jpeg,&jpeg_size);
		return jpeg_size;
}

/*head[1] ver[1] re[1] cmd[1] payloadlen[4] payload[n] check[2]*/
typedef struct{
	uint8_t head;
	uint8_t version;
	uint8_t err;//0 ok 1err
	uint8_t cmd;//0 为实现   1获取jpeg数据 payload[0] 几幅图像
	uint32_t  payload_size;
}tcp_jpeg_stream_head;

static int listenAtTcpAddr(uint16_t Port) {
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(Port);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  listen(sock, 1);
  return sock;
}

static struct sockaddr_in clientAddr;

BaseType_t xReturnedTcpServer;
TaskHandle_t xHandleTcpServer = NULL;
void task_tcp_server(void * pvParameters)
{
	uint16_t Port = 9000;
  int s,Sock;
	uint32_t AddrLen;
	int data_len=0;
	uint8_t *jpeg_data=0;
	uint8_t *payload=0;
	uint16_t cnt=0;
	
	uint8_t *rx_buf = staticMalloc(32);
	uint8_t *tx_buf = 0;
	tcp_jpeg_stream_head *head=0;

	
  do {
    s = listenAtTcpAddr(Port);
    if (s != -1) {
      break;
    }
    osDelay(100); // Try again
  } while (1);
	Ac_log("Listen socket %s\r\n",s);
	
	while(1)
	{
		AddrLen = sizeof(clientAddr);
    if ((Sock = accept(s, (struct sockaddr*)&clientAddr, &AddrLen)) == -1) {
      continue; // Error
    }
		char*p = inet_ntoa(clientAddr.sin_addr);
		Ac_log("%s connect\r\n",p);
    //
    // Run the actual server
    //
   
		while(1)
		{
			data_len = recv(Sock,rx_buf,1024,0);
			if(data_len>sizeof(tcp_jpeg_stream_head))
			{
				head = (tcp_jpeg_stream_head*)rx_buf;
				payload = rx_buf+sizeof(tcp_jpeg_stream_head);
					//
					//if(0==crc16(rx_buf,rev_len)) ok
				switch(head->cmd)
				{
					case 0://return  info
						
					break;
					
					case 1://return jpeg data
						if(head->payload_size!=1)continue;
						cnt = payload[0];
						while(cnt--)
						{
							data_len = app_jpeg_stream_out(&jpeg_data);
							tx_buf = pvPortMalloc(sizeof(tcp_jpeg_stream_head)+2 + data_len);
							head = (tcp_jpeg_stream_head*)tx_buf;
							head->head = 0xaa;
							head->version = 1;		
							head->cmd = 0x81;		
							head->payload_size = data_len;
							if(jpeg_data)//
							{									  
								memcpy(tx_buf+sizeof(tcp_jpeg_stream_head),jpeg_data,data_len);
								head->err = 0;							
							}
							else
							{
								head->err = 1;
							}			
							//
							//crc = crc16(tx_buf,sizeof(tcp_jpeg_stream_head)+ data_len)	
							//tx_buf[sizeof(tcp_jpeg_stream_head)+ data_len] = crc >>16;
							//tx_buf[sizeof(tcp_jpeg_stream_head)+ data_len +1]	=crc
							
							data_len=send(Sock,tx_buf,sizeof(tcp_jpeg_stream_head)+2 + data_len,0);
							vPortFree(tx_buf);
							if(data_len<0)
							{
								break;
							}
						}
					break;
										
					default:
						
					break;
				}
			}
			else
			{
				break;
			}
			
		}
    //
    // Close the connection
    //
    closesocket(Sock);
		Ac_log("%s close\r\n",p);
    memset(&clientAddr, 0, sizeof(struct sockaddr_in));		
	}
	
}

/*

*/

void task_creamra_jpeg_stream()
{
	uint32_t activeFrameAddr;
	int p=0;
		
//	creatbmp(320,240,2);
//	memcpy((U8*)bm0.pData,bm1.pData,480*272*2);
//	GUI_DrawBitmap(&bm0,0,0);
	
	xJpegstreamSemaphore = xSemaphoreCreateCounting(4,4);
	
	if(0==jpeg_stream_init())
	{
		while(1)
		{
			osDelay(1000);
		}
	}
	bsp_InitCamera();
  for(;;)
  {	
		camera_GetFullBuffer(&activeFrameAddr);			
		p=driver_jpeg_stream_in();				
		if(p>=0)
		{
			JPEG[p].size = rgb565_jpeg((void*)activeFrameAddr,&JPEG[p].jpeg);
			
		}
		else
		{
			xSemaphoreTake( xJpegstreamSemaphore, ( TickType_t ) 1000 );  
		}	
		CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, activeFrameAddr);
  }
}


/*

*/
__IO	uint32_t stream_cnt=0;
void task_creamra_jpeg_lcd_stream()
{
	uint32_t activeFrameAddr;
	uint32_t inactiveFrameAddr;
	uint32_t p=0;
	
	xJpegstreamSemaphore = xSemaphoreCreateCounting(4,4);
	
	if(0==jpeg_stream_init())
	{
		Ac_log("staticMalloc err\r\n");
		while(1)
		{
			osDelay(1000);
		}
	}
	
	bsp_InitCamera();
	LCD_Board_Init(1);
	
	camera_GetFullBuffer(&activeFrameAddr);
	camera_GetFullBuffer(&inactiveFrameAddr);
	
	LCD_SetNextBufferAddr(&cameraConfig,inactiveFrameAddr);		
	
  for(;;)
  {

		wait_lcd_frame_isr();		
		CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, activeFrameAddr);	
		activeFrameAddr = inactiveFrameAddr;	
		
		if(camera_GetFullBuffer(&inactiveFrameAddr)<0)continue;	
		
	
		p=tail;
		if(p==head)
		{
		//	xSemaphoreTake( xJpegstreamSemaphore, ( TickType_t ) 20 ); 
		}
		else
		{	
			JPEG[p].size=rgb565_jpeg((void*)inactiveFrameAddr,&JPEG[p].jpeg);	
			tail = p<FIFO_NUM-1 ? p+1 : 0;	

		}
				

		
		LCD_SetNextBufferAddr(&cameraConfig,inactiveFrameAddr);
		stream_cnt++;
  }
}



int rgb888_jpeg_compress(unsigned char *inbuf, unsigned char **outbuf,unsigned long *outSize)
{
	int jpegWidth = 320;
	int jpegHeight = 240;
	struct jpeg_compress_struct toWriteCinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	int row_stride;

	toWriteCinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&toWriteCinfo);

	jpeg_mem_dest(&toWriteCinfo, outbuf,outSize);

	toWriteCinfo.image_width = jpegWidth;
	toWriteCinfo.image_height = jpegHeight;
	toWriteCinfo.input_components = 3;//3
	toWriteCinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&toWriteCinfo);
	jpeg_set_quality(&toWriteCinfo, 75, TRUE);//0~100
	jpeg_start_compress(&toWriteCinfo,TRUE);
	row_stride = jpegWidth ;
	while(toWriteCinfo.next_scanline < toWriteCinfo.image_height)
	{
		row_pointer[0] = & inbuf[toWriteCinfo.next_scanline * row_stride * 3];//行首偏移地址
		(void)jpeg_write_scanlines(&toWriteCinfo, row_pointer, 1);
	}
	jpeg_finish_compress(&toWriteCinfo);
	jpeg_destroy_compress(&toWriteCinfo);
	return 0;
}
