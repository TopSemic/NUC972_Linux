#ifndef _V4L2_USE_H
#define _V4L2_USE_H

/*******头文件**********/
#include <stdio.h>
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>           
#include <fcntl.h>             
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>         
#include <linux/videodev2.h>

#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>

#include "lcd.h"

typedef struct v4l2_result{
	void* jpeg_buf;
	unsigned char* stre_buf;
	long jpeg_buf_size;
} V4L2_STRU;



/*******宏**********/
#define CLEAR(x) memset(&(x), 0, sizeof(x))//清内存

#define WIDTH 640 // 源图片宽度
#define HEIGHT 480 // 源图片高度
#define IMG_SIZE (WIDTH * HEIGHT * 3) // 源图片大小
#define BITCOUNT 24  // 源图片位深读


#define DEST_WIDTH 320 // 放大后目标图片宽度 320 240
#define DEST_HEIGHT 240 //目标图片高度 
#define DEST_IMG_SIZE (DEST_WIDTH * DEST_HEIGHT * 3) // 目标图片大小
#define DEST_BUF_SIZE (((DEST_WIDTH * BITCOUNT +31) / 32) * 4 * DEST_HEIGHT)//目标图片大小 



/****yuyv **********/ 
struct buffer{
	char * start;
	size_t length;
};

typedef struct v4l2 {
	unsigned char *dest_buffers;
	unsigned char *rgb_buffers;
	void *jpeg;
	long jpeg_size;
	
	void (*open)(void);
	void (*read)(void);
	void (*write)(str_lcd *lcdone, char *buf, int size);
	void (*close)(str_lcd *lcdone);
} V4L2;




static int fd = -1; //yuyv 设备文件流
static struct buffer* yuyv_buffers0 = NULL ;
static unsigned int n_buffers = 0;

static void v4l2_lcd_close(str_lcd *lcdone);

static void v4l2_lcd_init(void);
static int v4l2_open(char* dev_name);
static int v4l2_capture_rgb(void);
static int read_frame(void);
static void v4l2_close(void);

extern V4L2 v4l2_use;
 



#endif
