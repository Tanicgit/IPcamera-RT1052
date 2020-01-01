#include "tcp_clinet.h"
#include "lwip/opt.h"
#include "lwip/api.h"

#include "bsp.h"
struct netconn *tcp_clientconn;					//TCP CLIENT网络连接结构体
uint8_t tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP客户端接收数据缓冲区
uint8_t *tcp_client_sendbuf="Explorer STM32F407 NETCONN TCP Client send data\r\n";	//TCP客户端发送数据缓冲区
uint8_t tcp_client_flag;		//TCP客户端数据发送标志位

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
		tcp_clientconn = netconn_new(NETCONN_TCP);  //创建一个TCP链接
		err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);//连接服务器
		if(err != ERR_OK)  netconn_delete(tcp_clientconn); //返回值不等于ERR_OK,删除tcp_clientconn连接
		else if (err == ERR_OK)    //处理新连接的数据
		{ 
			struct netbuf *recvbuf;
			tcp_clientconn->recv_timeout = 10;
			netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1); //获取本地IP主机IP地址和端口号
			PRINTF("连接上服务器%d.%d.%d.%d,本机端口号为:%d\r\n",192,168,0,104,loca_port);
			while(1)
			{
				osDelay(20);
			//	if((tcp_client_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要发送
				if(cnt==100)
				{
					cnt=0;
					err = netconn_write(tcp_clientconn ,tcp_client_sendbuf,strlen((char*)tcp_client_sendbuf),NETCONN_COPY); //发送tcp_server_sentbuf中的数据
					if(err != ERR_OK)
					{
						PRINTF("发送失败\r\n");
					}
					tcp_client_flag &= ~LWIP_SEND_DATA;
				}
				cnt++;	
				recv_err = netconn_recv(tcp_clientconn,&recvbuf);
				if(recv_err == ERR_OK)  //接收到数据
				{	
				//	OS_ENTER_CRITICAL(); //关中断
					memset(tcp_client_recvbuf,0,TCP_CLIENT_RX_BUFSIZE);  //数据接收缓冲区清零
					for(q=recvbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
					{
						//判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于
						//的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
						if(q->len > (TCP_CLIENT_RX_BUFSIZE-data_len)) memcpy(tcp_client_recvbuf+data_len,q->payload,(TCP_CLIENT_RX_BUFSIZE-data_len));//拷贝数据
						else memcpy(tcp_client_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > TCP_CLIENT_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
					}
				//	OS_EXIT_CRITICAL();  //开中断
					data_len=0;  //复制完成后data_len要清零。					
					PRINTF("%s\r\n",tcp_client_recvbuf);
					netbuf_delete(recvbuf);
				}else if(recv_err == ERR_CLSD)  //关闭连接
				{
					netconn_close(tcp_clientconn);
					netconn_delete(tcp_clientconn);
					PRINTF("服务器%d.%d.%d.%d断开连接\r\n",192,168, 0,104);
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
