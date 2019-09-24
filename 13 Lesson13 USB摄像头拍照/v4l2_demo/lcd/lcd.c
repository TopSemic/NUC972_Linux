#include "lcd.h"
#include "v4l2_use.h"


/*
lcd设备初始化函数，参数传设备名和路径
返回自定义设备参数结构体的指针
*/
str_lcd *lcd_init(char *dev_name)
{
	static str_lcd *fb_lcd=NULL;
	fb_lcd=(str_lcd *)malloc(sizeof(str_lcd));
	if(!fb_lcd)
	{
		return NULL;
	}
	fb_lcd->fbfd=open(dev_name,O_RDWR);
	if(-1==fb_lcd->fbfd)
	{
		printf("open lcd faile\n");
		return NULL;
	}
	if(ioctl(fb_lcd->fbfd, FBIOGET_VSCREENINFO, &fb_lcd->vinfo)<0)
	{
		printf("ioctl lcd FBIOGET_VSCREENINFO faile\n");
		return NULL;
	}
	if(ioctl(fb_lcd->fbfd, FBIOGET_FSCREENINFO, &fb_lcd->finfo)<0)
	{
		printf("ioctl lcd FBIOGET_FSCREENINFO faile\n");
		return NULL;
	}
//	printf("lcd screen x=%d,y=%d\n",fb_lcd->vinfo.xres,fb_lcd->vinfo.yres);

	fb_lcd->screensize=(fb_lcd->vinfo.xres*fb_lcd->vinfo.yres*fb_lcd->vinfo.bits_per_pixel)/8;
	fb_lcd->fbp=(char *)mmap(0,fb_lcd->screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fb_lcd->fbfd,0);
	if(fb_lcd->fbp==MAP_FAILED)
	{
		printf("lcd memorymap faile\n");
		return NULL;
	}
	
	//打印一些参数信息
	printf("lcd screen x=%d,y=%d\n lcd screensize = %ld\n lcd bits = %d\n", fb_lcd->vinfo.xres,\
								fb_lcd->vinfo.yres, fb_lcd->screensize, fb_lcd->vinfo.bits_per_pixel);
	
	return fb_lcd;
}

/*
在lcd中显示图片，
参数一设备结构体指针，参数二图片数据的首地址，图片数据的字节数320*240*3。
*/
void lcd_display(str_lcd *lcdone, char *buf, int size)
{	
	int ss = 0;
	int i = 0;
	int num = 0;
	if((size == 176 * 144 * 3)) {
		for(i = 0; i < size;)
		{
		   if(num == WIDTH)
			{
				num = 0;
				ss += (320 - WIDTH)*4;
			}
			*(lcdone->fbp+ss)= buf[i+2];// b
			*(lcdone->fbp+ss+1)= buf[i];//r
			*(lcdone->fbp+ss+2)= (buf[i+1] );//g
			*(lcdone->fbp+ss+3)= 0x00;
			num++;
			ss += 4;
			i += 3;
			if(ss >= lcdone->screensize)
			{ break;}	
		}
	} else {
		for(i = 0; i < size;)
		{
			*(lcdone->fbp+ss)= buf[i+2];// b
			*(lcdone->fbp+ss+1)= buf[i];//r
			*(lcdone->fbp+ss+2)= (buf[i+1] );//g
			*(lcdone->fbp+ss+3)= 0x00;
			
			ss += 4;
			i += 3;
			if(ss >= lcdone->screensize)
			{ break;}	
		}
	}
	//usleep(300000);     
}
/* 关闭，参数传设备结构体。*/
void lcd_close(str_lcd *lcdone)
{
	 munmap(lcdone->fbp, lcdone->screensize);
     close(lcdone->fbfd);
}




