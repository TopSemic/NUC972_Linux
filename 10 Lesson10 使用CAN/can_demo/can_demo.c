/***********************************************
 * @{
 * @file  : can_demo.c
 * @brief : 
 * @author: Wenxue Wang
 * @email : topsemic@sina.com
 * @date  : 2019-07-28 
***********************************************/

//--------------------------------------------------
// Copyright (c) Topsemic
//--------------------------------------------------

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// CAN socket
int socketfd = -1;

// CAN filter
struct can_filter *filter = NULL;

// CAN ID number in filter
int filter_count = 0;

int can_init(char* can_if)
{
	if(socketfd != -1)
	{
		return -1;
	}

	struct ifreq ifr;
	struct sockaddr_can addr;

	int family = PF_CAN, type = SOCK_RAW, proto = CAN_RAW;
	
	// Initialize socket
	if ((socketfd = socket(family, type, proto)) < 0) {
		perror("socket");
		return -2;
	}

	addr.can_family = family;
	strncpy(ifr.ifr_name, can_if, sizeof(ifr.ifr_name));
	if (ioctl(socketfd, SIOCGIFINDEX, &ifr)) {
		perror("ioctl");
		return -3;
	}
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return -4;
	}

	// can filter
	if (filter && filter_count > 0)
	{
		if (setsockopt(socketfd, SOL_CAN_RAW, CAN_RAW_FILTER, filter,filter_count * sizeof(struct can_filter)) != 0)
		{
			perror("setsockopt");
			exit(1);
		}
	}
	return 1;
}

int add_standard_filter(u_int32_t can_id)
{
	int i;
	for(i=0;i<filter_count;i++)
	{
		if(filter[i].can_id == can_id)
			return 0;
	}

	filter = (struct can_filter *)realloc(filter, sizeof(struct can_filter) * (filter_count + 1));
	if(!filter)
		return -1;

	filter[filter_count].can_id = can_id;
	filter[filter_count].can_mask = CAN_SFF_MASK;
	filter_count ++;

	return 1;
}

int add_extended_filter(u_int32_t can_id)
{
	int i;
	for(i=0;i<filter_count;i++)
	{
		if(filter[i].can_id == can_id)
			return 0;
	}

	filter = (struct can_filter *)realloc(filter, sizeof(struct can_filter) * (filter_count + 1));
	if(!filter)
		return -1;

	filter[filter_count].can_id = can_id;
	filter[filter_count].can_mask = CAN_EFF_MASK;
	filter_count ++;

	return 1;
}

int can_send(struct can_frame* pframe)
{
	if(socketfd < 0)
		return -1;

	if(pframe == NULL)
		return -2;

	int res = send(socketfd, pframe, sizeof(struct can_frame), 0);

	return res;
}


int main(void)
{
	if(can_init("can0") < 0)
	{
		printf("init can socket failed!\r\n");
		return -1;
	}
	
	// send can
	struct can_frame frame;
	memset(&frame, 0x00, sizeof(frame));
	frame.can_id = 0x123;
	frame.can_dlc = 8;
	unsigned char data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
	memcpy(frame.data, data, 8);
    can_send(&frame);

	
	int nbytes;
	int index = 0;

	// read can
	while(1)
	{
		memset(&frame, 0, sizeof(struct can_frame));
		if ((nbytes = read(socketfd, &frame, sizeof(struct can_frame))) != sizeof(struct can_frame))
		{
			perror("read");
		}
		else
		{
			printf("CANID : 0x%x : Data : ",frame.can_id);
			for (index = 0; index < frame.can_dlc; index++)
			{
				printf("0x%x ", frame.data[index]);
			}
			printf("\n");
		}

		usleep (1000);
	}
	return 0;
}

