#include "tcp_clinet.h"
#include "lwip/opt.h"
#include "lwip/api.h"

#include "bsp.h"
struct netconn *tcp_clientconn;					//TCP CLIENT�������ӽṹ��
uint8_t tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP�ͻ��˽������ݻ�����
uint8_t *tcp_client_sendbuf="Explorer STM32F407 NETCONN TCP Client send data\r\n";	//TCP�ͻ��˷������ݻ�����
uint8_t tcp_client_flag;		//TCP�ͻ������ݷ��ͱ�־λ

static void tcp_client_thread(void *arg)
{

	uint32_t data_len = 0,cnt=0;;
	struct pbuf *q;
	err_t err,recv_err;
	static ip_addr_t server_ipaddr,loca_ipaddr;
	static u16_t 		 server_port,loca_port;

	LWIP_UNUSED_ARG(arg);
	server_port = 777;
	IP4_ADDR(&server_ipaddr, 192,168, 0,104);
	
	while (1) 
	{
		tcp_clientconn = netconn_new(NETCONN_TCP);  //����һ��TCP����
		err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);//���ӷ�����
		if(err != ERR_OK)  netconn_delete(tcp_clientconn); //����ֵ������ERR_OK,ɾ��tcp_clientconn����
		else if (err == ERR_OK)    //���������ӵ�����
		{ 
			struct netbuf *recvbuf;
			tcp_clientconn->recv_timeout = 10;
			netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1); //��ȡ����IP����IP��ַ�Ͷ˿ں�
			PRINTF("�����Ϸ�����%d.%d.%d.%d,�����˿ں�Ϊ:%d\r\n",192,168,0,104,loca_port);
			while(1)
			{
				osDelay(20);
			//	if((tcp_client_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
				if(cnt==100)
				{
					cnt=0;
					err = netconn_write(tcp_clientconn ,tcp_client_sendbuf,strlen((char*)tcp_client_sendbuf),NETCONN_COPY); //����tcp_server_sentbuf�е�����
					if(err != ERR_OK)
					{
						PRINTF("����ʧ��\r\n");
					}
					tcp_client_flag &= ~LWIP_SEND_DATA;
				}
				cnt++;	
				recv_err = netconn_recv(tcp_clientconn,&recvbuf);
				if(recv_err == ERR_OK)  //���յ�����
				{	
				//	OS_ENTER_CRITICAL(); //���ж�
					memset(tcp_client_recvbuf,0,TCP_CLIENT_RX_BUFSIZE);  //���ݽ��ջ���������
					for(q=recvbuf->p;q!=NULL;q=q->next)  //����������pbuf����
					{
						//�ж�Ҫ������TCP_CLIENT_RX_BUFSIZE�е������Ƿ����TCP_CLIENT_RX_BUFSIZE��ʣ��ռ䣬�������
						//�Ļ���ֻ����TCP_CLIENT_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
						if(q->len > (TCP_CLIENT_RX_BUFSIZE-data_len)) memcpy(tcp_client_recvbuf+data_len,q->payload,(TCP_CLIENT_RX_BUFSIZE-data_len));//��������
						else memcpy(tcp_client_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_CLIENT_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
					}
				//	OS_EXIT_CRITICAL();  //���ж�
					data_len=0;  //������ɺ�data_lenҪ���㡣					
					PRINTF("%s\r\n",tcp_client_recvbuf);
					netbuf_delete(recvbuf);
				}else if(recv_err == ERR_CLSD)  //�ر�����
				{
					netconn_close(tcp_clientconn);
					netconn_delete(tcp_clientconn);
					PRINTF("������%d.%d.%d.%d�Ͽ�����\r\n",192,168, 0,104);
					break;
				}
				
			}
		}
	}
}


void
tcp_client_init(void)
{
  sys_thread_new("tcp_client_thread", tcp_client_thread, NULL, 1024, 3);
}
