#include "app_dhcp.h"
#include "bsp.h"

#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "ethernetif.h"

#include "lwip/tcpip.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 108

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 0
#define configGW_ADDR3 100

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk

/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

static uint8_t print_dhcp_state(struct netif *netif)
{
    static u8_t dhcp_last_state = DHCP_STATE_OFF;
    struct dhcp *dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
		uint8_t re=0;
    if (dhcp_last_state != dhcp->state)
    {
        dhcp_last_state = dhcp->state;

        PRINTF(" DHCP state       : ");
        switch (dhcp_last_state)
        {
            case DHCP_STATE_OFF:
                PRINTF("OFF");
                break;
            case DHCP_STATE_REQUESTING:
                PRINTF("REQUESTING");
                break;
            case DHCP_STATE_INIT:
                PRINTF("INIT");
                break;
            case DHCP_STATE_REBOOTING:
                PRINTF("REBOOTING");
                break;
            case DHCP_STATE_REBINDING:
                PRINTF("REBINDING");
                break;
            case DHCP_STATE_RENEWING:
                PRINTF("RENEWING");
                break;
            case DHCP_STATE_SELECTING:
                PRINTF("SELECTING");
                break;
            case DHCP_STATE_INFORMING:
                PRINTF("INFORMING");
                break;
            case DHCP_STATE_CHECKING:
                PRINTF("CHECKING");
                break;
            case DHCP_STATE_BOUND:
                PRINTF("BOUND");
								re=1;
                break;
            case DHCP_STATE_BACKING_OFF:
                PRINTF("BACKING_OFF");
                break;
            default:
                PRINTF("%u", dhcp_last_state);
                assert(0);
                break;
        }
        PRINTF("\r\n");
    }
		return re;
}

static struct netif fsl_netif0;
struct netif *netif = &fsl_netif0;
void lwip_ip_get()
{
	uint8_t m=0,re=0;
	ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
	ethernetif_config_t fsl_enet_config0 = {
		.phyAddress = 0,
		.clockName = EXAMPLE_CLOCK_NAME,
		.macAddress = configMAC_ADDR,
	};

	IP4_ADDR(&fsl_netif0_ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&fsl_netif0_netmask, 0, 0, 0, 0);
	IP4_ADDR(&fsl_netif0_gw, 0, 0, 0, 0);

	tcpip_init(NULL, NULL);

	netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw,
		&fsl_enet_config0, ethernetif0_init, tcpip_input);
	netif_set_default(&fsl_netif0);
	
//	netif_set_down(&fsl_netif0);
	
	netif_set_up(&fsl_netif0);	

	dhcp_start(&fsl_netif0);
	PRINTF("******************<DHCP>************************\r\n");
	while(re==0)
	{
		osDelay(1000);	
		re=print_dhcp_state(&fsl_netif0);			
		m++;
	}

	if(m>=15)
	{
		IP4_ADDR(&(fsl_netif0.ip_addr), configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
		IP4_ADDR(&(fsl_netif0.netmask), configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
		IP4_ADDR(&(fsl_netif0.gw), configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);						
	}
	PRINTF("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif->ip_addr.addr)[0],
		((u8_t *)&netif->ip_addr.addr)[1], ((u8_t *)&netif->ip_addr.addr)[2],
		((u8_t *)&netif->ip_addr.addr)[3]);
	PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif->netmask.addr)[0],
		((u8_t *)&netif->netmask.addr)[1], ((u8_t *)&netif->netmask.addr)[2],
		((u8_t *)&netif->netmask.addr)[3]);
	PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n\r\n", ((u8_t *)&netif->gw.addr)[0],
		((u8_t *)&netif->gw.addr)[1], ((u8_t *)&netif->gw.addr)[2], 
		((u8_t *)&netif->gw.addr)[3]);
	PRINTF("******************<END>*************************\r\n");
}

