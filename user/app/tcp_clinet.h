#ifndef __TCP_CLINET_H
#define __TCP_CLINET_H

#define TCP_CLIENT_RX_BUFSIZE	2000	//���ջ���������
#define REMOTE_PORT				8087	//����Զ��������IP��ַ
#define LWIP_SEND_DATA			0X80    //���������ݷ���


void tcp_client_init(void);  //tcp�ͻ��˳�ʼ��(����tcp�ͻ����߳�)

#endif

