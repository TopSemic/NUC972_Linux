#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/acpi.h>
#include <linux/platform_device.h>
#include <mach/gpio.h>
#include <linux/clk.h>
#include <mach/map.h>
#include <mach/regs-gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-gcr.h>
#include <mach/irqs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <mach/regs-gpio.h>
#include <asm/io.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
#include <asm/uaccess.h>

#define u32 	unsigned long int 
#define u16 	unsigned int 
#define u8 	unsigned char

#define s32  	long  int
#define s16 	int
#define s8 	    char


#define	SMPLRT_DIV		0x19	
#define	MPUCONFIG		0x1A	
#define	GYRO_CONFIG		0x1B	
#define	ACCEL_CONFIG	0x1C
#define INT_PIN_CFG     0x37
#define INT_ENABLE      0x38	
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define USER_CTRL       0x6A
#define	PWR_MGMT_1		0x6B	
#define	WHO_AM_I	    0x75
#define	SlaveAddress	0xD0	

u8 InitMPU6050(void);
extern u8 mpu_data[14];
void mpu_get_data(void);



