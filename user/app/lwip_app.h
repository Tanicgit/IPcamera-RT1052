#ifndef __LWIP_APP_H
#define __LWIP_APP_H
	#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "ethernetif.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "lwip/prot/dhcp.h"
#include "lwip/init.h"
#include "lwip/netif.h"
	#if defined ( __CC_ARM )  /* MDK ARM Compiler */
	#include "lwip/sio.h"
	#endif
void stack_init(void *arg);
void LWIP_Init(void);
uint8_t CheckDHCPsta(void);
#endif
