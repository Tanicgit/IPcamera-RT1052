#ifndef __TCP_CLINET_H
#define __TCP_CLINET_H

#define TCP_CLIENT_RX_BUFSIZE	2000	//接收缓冲区长度
#define REMOTE_PORT				8087	//定义远端主机的IP地址
#define LWIP_SEND_DATA			0X80    //定义有数据发送


void tcp_client_init(void);  //tcp客户端初始化(创建tcp客户端线程)

#endif

