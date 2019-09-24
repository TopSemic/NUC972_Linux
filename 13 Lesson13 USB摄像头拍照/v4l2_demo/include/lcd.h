#ifndef __LCD_H
#define __LCD_H

#include <errno.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdint.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <malloc.h>
#include <linux/fb.h>
#include <ctype.h>
#include <sys/mman.h>
#include <stropts.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct lcd{	
struct fb_var_screeninfo vinfo;	  // LCD可变参数
struct fb_fix_screeninfo finfo;	  // LCD固定参数	
char *fbp;  //映射地址	
int fbfd;	//lcd描述符
long int screensize;//屏幕所占内存大小
}str_lcd;


str_lcd *lcd_dev_parmet;//自定义设备参数结构体的指针

str_lcd *lcd_init(char *dev_name);

void lcd_display(str_lcd *lcdone, char *buf, int size);

void lcd_close(str_lcd *lcdone);


#endif
