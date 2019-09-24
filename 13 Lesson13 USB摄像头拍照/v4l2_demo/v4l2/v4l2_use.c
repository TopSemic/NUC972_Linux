#include "v4l2_use.h"

#include "image_process.h"

 V4L2 v4l2_use = {	
	.open = v4l2_lcd_init,
	.read = v4l2_capture_rgb,
	.write = lcd_display,
	.close = v4l2_lcd_close,
};

static void v4l2_lcd_close(str_lcd *lcdone)
{
	v4l2_close();
	lcd_close(lcdone);
}

static void v4l2_lcd_init(void)
{
	char* v4l2_dev_name = "/dev/video0";//摄像头设备名
	//char* lcd_dev_name = "/dev/fb0";//LCD设备名
	

	unsigned char* all_rgb_buffers = (unsigned char*) malloc(WIDTH * HEIGHT * 3);//rgb 数据存储区
	unsigned char* all_dest_buffers = (unsigned char*) malloc(DEST_BUF_SIZE); //((des_width * BITCOUNT +31) / 32) * 4 * des_hight;

	v4l2_use.dest_buffers = all_dest_buffers;
	v4l2_use.rgb_buffers = all_rgb_buffers;
	
	int flag = v4l2_open(v4l2_dev_name);//打开v4l2
	if(-1 ==flag){
		printf("open /dev/video0 failed!\n");
		exit(-1);
	}
        else
                printf("open /dev/video success!\n");

	/*lcd_dev_parmet = lcd_init(lcd_dev_name);//打开LCD
	if(NULL == lcd_dev_parmet){
		printf("open /dev/fb0 failed!\n");
		exit(-1);
	} */

}

static int v4l2_open(char* dev_name)
{
	int ret = 0;

	struct v4l2_capability cap;
	struct v4l2_format fmt;
	unsigned int i = 0;
	enum v4l2_buf_type type;

	fd = open(dev_name, O_RDWR, 0 );//打开设备文件,阻塞模式
	if (fd < 0){
		  perror("open /dev/video0  fialed! ");
		  return -1;
	}
        else
            printf("v4l2_open function  open /dev/video0 success!\n");

	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);//查看设备功能
	if (ret < 0){
		perror("requre VIDIOC_QUERYCAP fialed! \n");
		return -1;
	}

	printf("driver:\t\t%s\n",cap.driver);
    printf("card:\t\t%s\n",cap.card);
    printf("bus_info:\t%s\n",cap.bus_info);
    printf("version:\t%d\n",cap.version);
    printf("capabilities:\t%x\n",cap.capabilities);
			
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE){
		printf("Device %s: supports capture.\n",dev_name);
	}
	if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING){
		printf("Device %s: supports streaming.\n",dev_name);
	}

	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	fmt.fmt.pix.width = WIDTH;
	fmt.fmt.pix.height = HEIGHT;

	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt)){//设置图片格式
		perror("set format failed!");
		return -1;
	}
	if(-1 == ioctl(fd, VIDIOC_G_FMT, &fmt)){//得到图片格式
		perror("set format failed!");
		return -1;
	}

	printf("fmt.type:\t\t%d\n",fmt.type);
	printf("pix.pixelformat:\t%c%c%c%c\n", \
			fmt.fmt.pix.pixelformat & 0xFF,\
			(fmt.fmt.pix.pixelformat >> 8) & 0xFF, \
			(fmt.fmt.pix.pixelformat >> 16) & 0xFF,\
			(fmt.fmt.pix.pixelformat >> 24) & 0xFF);
	printf("pix.width:\t\t%d\n",fmt.fmt.pix.width);
	printf("pix.height:\t\t%d\n",fmt.fmt.pix.height);
	printf("pix.field:\t\t%d\n",fmt.fmt.pix.field);

	struct v4l2_requestbuffers req;
	CLEAR(req);
	req.count = 5;//申请缓冲数量
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ioctl(fd, VIDIOC_REQBUFS, &req);//申请缓冲，
	if (req.count < 2){
		perror("buffer memory is Insufficient! \n");
		return -1;
	}

        yuyv_buffers0 = calloc(req.count, sizeof(*yuyv_buffers0));//内存中建立对应空间
	
for (n_buffers = 0; n_buffers < req.count; ++n_buffers){
		struct v4l2_buffer buf;//驱动中的一帧
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;

		if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)){//映射用户空间
			perror("VIDIOC_QUERYBUF error!\n");
			return -1;
		}

		yuyv_buffers0[n_buffers].length = buf.length;
		yuyv_buffers0[n_buffers].start =(char*) mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);/*start anywhere*//*recommended*/

		if (MAP_FAILED == yuyv_buffers0[n_buffers].start){
			close(fd);
			perror("mmap faild! \n");
			return -1;
		}

		printf("Frame buffer %d: address = 0x%x, length = %d \n",req.count, (unsigned int)yuyv_buffers0[n_buffers].start, yuyv_buffers0[n_buffers].length);
	}

	for (i=0; i<n_buffers; ++i){
		struct v4l2_buffer buf;
		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if ( -1 == ioctl(fd, VIDIOC_QBUF, &buf)){//申请到的缓冲进入队列
			close(fd);
			perror("VIDIOC_QBUF failed! \n");			
			return -1;
		}
	}
	
	debug("begen capture");	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl(fd, VIDIOC_STREAMON, &type)){//开始捕捉图像数据
		close(fd);
		perror("VIDIOC_STREAMON failed!  ");
		exit(-1);
	}	

	return 1;
}


static int v4l2_capture_rgb(void)
{
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fd_set fds;
	struct timeval tv;
	int r;

	FD_ZERO(&fds);//将指定文件描述符集清空
	FD_SET(fd, &fds);//在文件描述符集合中增加一个新的文件描述符
	tv.tv_sec = 2;//time out
	tv.tv_usec = 0;
	r = select(fd+1, &fds, NULL, NULL, &tv);//判断摄像头是否准备好，tv是定时

	if(-1 == r){
		if(EINTR == errno){
			printf("select erro! \n");
		}
	}
	else if(0 == r){
		printf("-------------select timeout! \n");//超时
		return 1;
		//exit(EXIT_FAILURE);
	}
	
	read_frame(); //执行read_frame() 函数
}

int numb;
static int read_frame(void)
{
	struct v4l2_buffer buf;
 	int ret =0;
	static char path1[30];
	static char path2[30];
	FILE *file_fd;//yuyv 图片文件流
	FILE *bmp_fd;//bmp 图片文件流 
	
	printf("------------- %d\n", numb++);
	
	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(fd, VIDIOC_DQBUF, &buf);//出列采集的帧缓冲,成功返回0
	if(0 != ret){
		printf("VIDIOC_DQBUF failed!\n");
		exit(-1);
	}
	
        printf("call yuyv_to_rgb function\n");
	yuyv_to_rgb(yuyv_buffers0[buf.index].start, v4l2_use.rgb_buffers);//yuyv -> rgb
	
        printf("yuyv_to_rgb function excute OK!\n");
	v4l2_use.jpeg_size = rgb_to_jpeg(v4l2_use.rgb_buffers, &v4l2_use.jpeg);  
        printf("1111111111111111111111111111111111\n");   
        //rgb_stretch(v4l2_use.rgb_buffers, v4l2_use.dest_buffers, DEST_WIDTH, DEST_HEIGHT);//176 X 144 -> 320 X 240 
		
	ret = ioctl(fd, VIDIOC_QBUF,&buf);//帧缓冲入列
	if(0 != ret){
		printf("VIDIOC_QBUF failed!\n");
		exit(-1);
	}
	return 1;
}

static void v4l2_close(void)
{
	int i=0;
unmap:
	for(i=0; i<n_buffers; ++i){
		if(-1 == munmap(yuyv_buffers0[i].start, yuyv_buffers0[i].length)){
			printf("munmap error! \n");
			exit(-1);
		}
	}
	close(fd);
	free(v4l2_use.rgb_buffers);
	free(v4l2_use.dest_buffers);
	exit(EXIT_SUCCESS);
}
