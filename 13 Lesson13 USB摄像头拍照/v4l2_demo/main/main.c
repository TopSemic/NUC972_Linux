
#include "v4l2_use.h"
#include "lcd.h"


int main()
{

	unsigned short port = 8000;//udp 初始化
	char *ipstr = "192.168.1.111";
	
	int server_so = socket(PF_INET, SOCK_DGRAM, 0);
	if(0 > server_so)
	{
		perror("socket");
		return -1;
	}
	
	struct sockaddr_in addr = {
		.sin_family = PF_INET,
		.sin_port = htons(port),
		.sin_addr.s_addr = inet_addr(ipstr),
	};	
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));	
	printf("----------- connected --------------\n");

	
	v4l2_use.open();
	
	for(;;)
	{	
		v4l2_use.read();
	//	v4l2_use.write(lcd_dev_parmet, v4l2_use.dest_buffers, DEST_IMG_SIZE);
		
	//	send_pic(server_so,addr);	
		
		printf("---------------------------\n");
	}
	v4l2_use.close(lcd_dev_parmet);	
	

  	
}

int send_pic(int so,struct sockaddr_in addr)
{	
    socklen_t len = sizeof(struct sockaddr_in);	
	int open_fd = 0;

	struct stat bf;
	char str[50]={0};
	
	int lll = sendto(so, v4l2_use.jpeg, v4l2_use.jpeg_size, 0, (struct sockaddr *) &addr, len);
	bzero(str,50);
	

}





