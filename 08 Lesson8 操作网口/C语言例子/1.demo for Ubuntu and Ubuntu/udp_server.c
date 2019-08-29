/***********************************************
 * @{
 * @file  : udp_server.c
 * @brief : 
 * @author: TopSemic
 * @email : topsemic@sina.com
 * @date  : 2019-06-20
***********************************************/

//--------------------------------------------------
// Copyright (c) Topsemic
//--------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <arpa/inet.h> 

#define LOCAL_RECV_PORT 8888

int main()
{
	int sockfd;
	int ret;
	struct sockaddr_in addr_local;
	struct sockaddr_in addr_sender;
	
	char buff[1024]={0x00};
	int nlen = sizeof(struct sockaddr);
	int recvlen = 0;	
    
    // sender address  
    bzero(&addr_sender,sizeof(addr_sender));
	
	// local address
	bzero(&addr_local,sizeof(addr_local));
	addr_local.sin_family=AF_INET; // AF_INET（又称 PF_INET）是 IPv4 网络协议的套接字类型
	addr_local.sin_addr.s_addr=htonl(INADDR_ANY);// INADDR_ANY 是指所有本机IP
	addr_local.sin_port= htons(LOCAL_RECV_PORT); // 绑定端口号

	sockfd=socket(AF_INET,SOCK_DGRAM,0); //UDP 创建套接字
	if(sockfd < 0) 
	{
        printf("create socket failure,sockfd:%d\n",sockfd);
		return -1;

	}
	
	ret=bind(sockfd,(struct sockaddr *)&addr_local,sizeof(addr_local));
	if(ret < 0) 
	{
		printf("bind failure\n");
		return -1;
	}

	while(1)
	{
		recvlen = recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&addr_sender,(socklen_t *)&nlen);
		if(recvlen > 0)
		{
			buff[recvlen] = 0x00;
			printf("udp server received data:%s\n",buff);
			printf("sender ip:%s port:%d\n",inet_ntoa(addr_sender.sin_addr),ntohs(addr_sender.sin_port));
			// 将接收到的数据原样发回去,发到接收到的IP和端口上
			sendto(sockfd, buff, recvlen, 0, (struct sockaddr *)& addr_sender, sizeof(struct sockaddr_in));
		}
        printf("**************************************\n");
	}
	close(sockfd);
	return 0;
}
