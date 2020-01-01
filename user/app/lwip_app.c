#include "t_shell.h"
#include "string.h"
#include "t_str.h"
#include "lwip_app.h"
#include "ethernetif.h"
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
void get_dhcp_state(struct netif *netif,char *info)
{
  u8_t dhcp_state = DHCP_STATE_OFF;
  struct dhcp *dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
	dhcp_state = dhcp->state;

	switch (dhcp_state)
	{
			case DHCP_STATE_OFF:
					strcpy(info,"OFF\r\n");
					break;
			case DHCP_STATE_REQUESTING:
					strcpy(info,"REQUESTING\r\n");
					break;
			case DHCP_STATE_INIT:
					strcpy(info,"INIT\r\n");
					break;
			case DHCP_STATE_REBOOTING:
					strcpy(info,"REBOOTING\r\n");
					break;
			case DHCP_STATE_REBINDING:
					strcpy(info,"REBINDING\r\n");
					break;
			case DHCP_STATE_RENEWING:
					strcpy(info,"RENEWING\r\n");
					break;
			case DHCP_STATE_SELECTING:
					strcpy(info,"SELECTING\r\n");
					break;
			case DHCP_STATE_INFORMING:
					strcpy(info,"INFORMING\r\n");
					break;
			case DHCP_STATE_CHECKING:
					strcpy(info,"CHECKING\r\n");
					break;
			case DHCP_STATE_BOUND:
					strcpy(info,"BOUND\r\n");
					break;
			case DHCP_STATE_BACKING_OFF:
					strcpy(info,"BACKING_OFF\r\n");
					break;
			default:
					strcpy(info,"STATE ERR\r\n");
					break;
	}
}
uint8_t CheckDHCPsta()
{
  u8_t dhcp_state = DHCP_STATE_OFF;
  struct dhcp *dhcp = (struct dhcp *)netif_get_client_data(&gnetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
	dhcp_state = dhcp->state;
	if(dhcp_state==DHCP_STATE_BOUND)return 1;
	else return 0;
}
void LWIP_Init(void)
{
		ethernetif_config_t fsl_enet_config0 = {
		.phyAddress = 0,
		.clockName = kCLOCK_CoreSysClk,
		.macAddress = configMAC_ADDR,
	};
  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* IP addresses initialization with DHCP (IPv4) */
//  ipaddr.addr = inet_addr("192.168.0.99");
//  netmask.addr = inet_addr("255.255.255.0");
//  gw.addr = inet_addr("192.168.0.1");
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
  /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, &fsl_enet_config0, &ethernetif0_init, &tcpip_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }
  dhcp_start(&gnetif);
}


/*
dhcp start
dhcp stop
dhcp sta
*/
int32_t DhcpCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	char buf[32];
	if(argc==2)
	{
		if( 0==strcmp(argv[1],"start"))
		{
				dhcp_start(&gnetif);			
				context->printf_data_func("DHCP BEGIN\r\n");
		}
		else if( 0==strcmp(argv[1],"stop"))
		{
				dhcp_stop(&gnetif);			
				context->printf_data_func("DHCP STOP\r\n");
		}
		else if( 0==strcmp(argv[1],"sta"))
		{
			get_dhcp_state(&gnetif,buf);
			context->printf_data_func(buf);
		}
		else
		{
			context->printf_data_func("UNKNOW!\r\n");
			context->printf_data_func("USED like this : dhcp []; []=start,stop,sta\r\n");
		}
	}
	
	return 0;
}

const shell_command_context_t xDhcpCommand = {"dhcp", "\r\n\"dhcp\":[start,stop,sta]\r\n",DhcpCommand, 1};


struct netif *netif = &gnetif;
int32_t IfconfigCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	context->printf_data_func("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif->ip_addr.addr)[0],
		((u8_t *)&netif->ip_addr.addr)[1], ((u8_t *)&netif->ip_addr.addr)[2],
		((u8_t *)&netif->ip_addr.addr)[3]);
	context->printf_data_func(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif->netmask.addr)[0],
		((u8_t *)&netif->netmask.addr)[1], ((u8_t *)&netif->netmask.addr)[2],
		((u8_t *)&netif->netmask.addr)[3]);
	context->printf_data_func(" IPv4 Gateway     : %u.%u.%u.%u\r\n\r\n", ((u8_t *)&netif->gw.addr)[0],
		((u8_t *)&netif->gw.addr)[1], ((u8_t *)&netif->gw.addr)[2], 
		((u8_t *)&netif->gw.addr)[3]);
	return 0;
}
const shell_command_context_t xIfconfigCommand = {"ifconfig", "\r\n\"ifconfig\": show IPv4 Address,Subnet mask and Gateway\r\n",IfconfigCommand, 0};



/*
netif sta
netif down
netif up
netif restart
*/
int32_t NetifCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	
	//STA
	if( 0==strcmp(argv[1],"sta"))
	{
		if(netif_is_link_up(&gnetif))
		{
			context->printf_data_func("netif_is_link_up OK\r\n");
		}
		else
		{
			context->printf_data_func("netif_is_link_up DOWN\r\n");
		}
	}
	//DOWN
	else if( 0==strcmp(argv[1],"down"))
	{	
		netif_set_down(&gnetif);	
	}
	//UP
	else if( 0==strcmp(argv[1],"up"))
	{	
		netif_set_up(&gnetif);	
	}
	//RESTART
	else if( 0==strcmp(argv[1],"restart"))
	{
		netif_set_down(&gnetif);
		netif_set_up(&gnetif);
		
	}
	else
	{
		context->printf_data_func("err\r\n");
	}
	return 0;
}
const shell_command_context_t xNetifCommand = {"netif", "\r\n\"netif\": [sta,down,up,restart]\r\n",NetifCommand, 1};

/*
ip set 192.168.0.102,255.255.255.0,192.168.0.1
*/
int32_t IpCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	_stringList *strlist=NULL;
	if( 0==strcmp(argv[1],"set"))
	{
		strlist=mtStrDiv(argv[2],",");
		if(strlist==NULL)
		{
			context->printf_data_func("err:strlist=NULL\r\n");
		}
		else if(strlist->num==3)
		{
			netif_set_down(&gnetif);
			
			ipaddr.addr = inet_addr(strlist->d[0]);
			netmask.addr = inet_addr(strlist->d[1]);
			gw.addr = inet_addr(strlist->d[2]);
			
			netif_set_addr(&gnetif, &ipaddr, &netmask, &gw);
			
			netif_set_up(&gnetif);
		}
		else
		{
			context->printf_data_func("config err\r\n");
		}
	}
	return 0;
}
const shell_command_context_t xIpCommand = {"ip", "\r\n\"ip\": [set]\r\n",IpCommand, 2};

/*
socket creat 			tcp 
socket connect 		0,192.168.0.106,3200
socket send 			0,asdfget
socket close 			0
socket jpeg       0
*/
#include "fsl_lpuart.h"
static struct sockaddr_in ip_addr;
int32_t SocketCommand(p_shell_context_t context, int32_t argc, char **argv)
{
	int port=-1;
	int type=-1;
	int re=-1;
	int s=-1;
	_stringList *strlist=NULL;
	
	//CREAT
	if(0==strcmp(argv[1],"creat"))
	{
		if(0==strcmp("tcp",argv[2]))
		{
			type = SOCK_STREAM;
		}
		else if(0==strcmp("udp",argv[2]))
		{
			type = SOCK_DGRAM;
		}
		else if(0==strcmp("raw",argv[2]))
		{
			type = SOCK_RAW;
		}
		else
		{
			context->printf_data_func("parameter(s) err\r\n");
		}
		if(type)
		{
			s = socket(AF_INET, type, 0);	
			if(s!=-1)
			{
				context->printf_data_func("creat ok,socket=%d\r\n",s);

			}
			else context->printf_data_func("creat err\r\n");			
		}
	}
	//CONNECT
	else if(0==strcmp(argv[1],"connect"))
	{
		strlist=mtStrDiv(argv[2],",");
		if(strlist==NULL)
		{
			context->printf_data_func("err:strlist=NULL\r\n");
		}
		else if(strlist->num!=3)
		{
			context->printf_data_func("err:strlist->num!=3\r\n");
		}
		else 
		{
			sscanf(strlist->d[0],"%d",&s);
			if(s>=0 && s<MEMP_NUM_NETCONN+LWIP_SOCKET_OFFSET)
			{
				sscanf(strlist->d[2],"%u",&port);
				if(port>0&&port<65536)
				{
					
					memset(&ip_addr, 0, sizeof(ip_addr));
					ip_addr.sin_family      = AF_INET;
					ip_addr.sin_port        = htons(port);
					ip_addr.sin_addr.s_addr = inet_addr(strlist->d[1]);
					//	bind(sock, (struct sockaddr *)&addr, sizeof(addr));						
					re = connect(s,(struct sockaddr *)(&ip_addr),sizeof(ip_addr));
					if(re==0)
					{
						context->printf_data_func("connect %s:%s ok\r\n",strlist->d[1],strlist->d[2]);
					}
					else
					{
						context->printf_data_func("connect %s:%s err\r\n",strlist->d[1],strlist->d[2]);
					}
				}
				else
				{
					context->printf_data_func("port err\r\n");
				}
				
			}
			else
			{
				context->printf_data_func("sock num err\r\n");
			}
		}
		FreeStringList(strlist);
		
	}
	//SNED
	else if(0==strcmp(argv[1],"send"))
	{
		strlist=mtStrDiv(argv[2],",");
		if(strlist==NULL)
		{
			context->printf_data_func("err:strlist=NULL\r\n");
		}
		else if(strlist->num!=2)
		{
			context->printf_data_func("err:strlist->num!=2\r\n");
		}
		else
		{
			sscanf(strlist->d[0],"%d",&s);
			if(s>=0 && s<MEMP_NUM_NETCONN+LWIP_SOCKET_OFFSET)
			{
				re = send(s,strlist->d[1],strlen(strlist->d[1]),0);
				if(re>=0)
				{
					context->printf_data_func("sock send %d,%s ok\r\n",s,strlist->d[1]);
				}
				else
				{
					context->printf_data_func("sock send %d,%s err\r\n",s,strlist->d[1]);
				}
			}
			else
			{
				context->printf_data_func("sock num err\r\n");
			}
		}
		FreeStringList(strlist);
	}
	//CLOSE
	else if(0==strcmp(argv[1],"close"))
	{	
		sscanf(argv[2],"%d",&s);
		if(s>=0 && s<MEMP_NUM_NETCONN+LWIP_SOCKET_OFFSET)
		{
			re = closesocket(s);
			if(re>=0)context->printf_data_func("close ok\r\n");
			else context->printf_data_func("close err\r\n");
		}
		else
		{
			context->printf_data_func("sock num err\r\n");
		}
	}
	//
	else if(0==strcmp(argv[1],"jpeg"))
	{
		#include "app_ui.h" 
		uint8_t *p=0;
		uint32_t jpeg_size=0;
		
		sscanf(argv[2],"%d",&s);
		if(s>=0 && s<MEMP_NUM_NETCONN+LWIP_SOCKET_OFFSET)
		{
			jpeg_size=app_jpeg_stream_out(&p);
			if(jpeg_size!=NULL)
			{
				
			//	re = send(s,p,jpeg_size,0);
				LPUART_WriteBlocking(LPUART1,(uint8_t*)p,jpeg_size);
				context->printf_data_func("sock jpeg ok\r\n");
			}
			else
			{
				context->printf_data_func("sock jpeg NULL\r\n");
			}
		}
		else
		{
			context->printf_data_func("sock num err\r\n");
		}
		
		

	}
	else
	{
		context->printf_data_func("err\r\n");
	}
	return 0;
}
const shell_command_context_t xSocketCommand = {"socket", "\r\n\"socket\": [cerat,connect,send,close,jpeg]\r\n",SocketCommand, 2};





