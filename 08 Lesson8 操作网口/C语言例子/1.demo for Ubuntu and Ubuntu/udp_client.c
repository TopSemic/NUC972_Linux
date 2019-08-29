/***********************************************
 * @{
 * @file  : udp_client.c
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

#define SEND_DEST_PORT 8888


int main()
{
	int sockfd;
	int ret;
	struct sockaddr_in addr_sender;
	struct sockaddr_in addr_dest;
	
	int nlen = sizeof(struct sockaddr);
	int recvlen=0;	
    
    // sender address  
    bzero(&addr_sender,sizeof(addr_sender));
		
	// dest address
	bzero(&addr_dest,sizeof(struct sockaddr_in));//每个字节都用0填充
	addr_dest.sin_family=AF_INET;  
    addr_dest.sin_addr.s_addr=inet_addr("127.0.0.1");
   	addr_dest.sin_port=htons(SEND_DEST_PORT); 

	sockfd=socket(AF_INET,SOCK_DGRAM,0); //udp 创建套接字
	if(sockfd < 0) 
	{
        printf("create socket failure,sockfd:%d\n",sockfd);
		return -1;

	}
	
	//不断获取用户输入并发送给服务器，然后接受服务器数据
	while(1)
	{
		char buff[1024] = {0x00};
		printf("Please Input a string: ");
		fgets(buff,1024,stdin);
		
		sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&addr_dest, sizeof(struct sockaddr_in));
		recvlen = recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&addr_sender,(socklen_t *)&nlen);
	    if(recvlen > 0)
		{
			buff[recvlen] = 0x00;
			printf("Message form server: %s\n", buff);
			printf("sender ip:%s port:%d\n",inet_ntoa(addr_sender.sin_addr),ntohs(addr_sender.sin_port));
		}
		printf("**************************************\n");
	}
	
	close(sockfd);
	return 0;
}
