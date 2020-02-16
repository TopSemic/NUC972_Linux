#include"mpu6050bsp.h"

u8 mpu_data[14];

// clock output high/low
#define mpu6050_SIC_H()     	*(volatile unsigned int *)REG_GPIOE_DATAOUT|=1<<11
#define mpu6050_SIC_L()     	*(volatile unsigned int *)REG_GPIOE_DATAOUT&=~(1<<11)

// data out high/low
#define mpu6050_SID_H()     	*(volatile unsigned int *)REG_GPIOE_DATAOUT|= 1<<10
#define mpu6050_SID_L()     	*(volatile unsigned int *)REG_GPIOE_DATAOUT&= ~(1<<10)

// data direction control
#define mpu6050_DATA_IN      	*(volatile unsigned int *)REG_GPIOE_DIR    &=~(1<<10)
#define mpu6050_DATA_OUT     	*(volatile unsigned int *)REG_GPIOE_DIR    |=  1<<10
#define mpu6050_CLOCK_OUT     	*(volatile unsigned int *)REG_GPIOE_DIR    |=  1<<11

#define mpu6050_SID_STATE	*(volatile unsigned int *)REG_GPIOE_DATAIN &  (1<<10)

void delayus(u8 a)
{
	udelay(2);
}
void mpu6050_Init(void)
{
	mpu6050_DATA_OUT;
	mpu6050_CLOCK_OUT;
   
	mpu6050_SID_H();	
	mpu6050_SIC_H();
}

void mpu6050_Start(void)
{
    mpu6050_SID_H();
    delayus(2);
    mpu6050_SIC_H();
    delayus(2);
    mpu6050_SID_L();
    delayus(2);
    mpu6050_SIC_L();
    delayus(2);
}


void mpu6050_Stop(void)
{
    mpu6050_SID_L();
    delayus(2);
    mpu6050_SIC_H();	
    delayus(2);  
    mpu6050_SID_H();	
    delayus(2);  
}

void noAck(void)
{	
	mpu6050_SID_H();	
	delayus(2);	
	mpu6050_SIC_H();	
	delayus(2);	
	mpu6050_SIC_L();	
	delayus(2);	
	mpu6050_SID_L();	
	delayus(2);
}

void Ack(void)
{	
	mpu6050_SID_L();	
	delayus(2);	
	mpu6050_SIC_H();	
	delayus(2);	
	mpu6050_SIC_L();	
	delayus(2);	
	mpu6050_SID_L();	
	delayus(2);
}

u8 mpu6050_Write(u8 m_data)
{
	u8 j,tem;

	for(j=0;j<8;j++)
	{
		if((m_data<<j)&0x80)
			mpu6050_SID_H();
		else 
			mpu6050_SID_L();
		delayus(2);
		mpu6050_SIC_H();	
		delayus(2);
		mpu6050_SIC_L();	
		delayus(2);
	}
	delayus(2);
	mpu6050_DATA_IN;
	delayus(2);
	mpu6050_SIC_H();	
	delayus(2);
	if(mpu6050_SID_STATE)
		tem=0;
	else 
		tem=1;
	mpu6050_SIC_L();	
	delayus(2);	
    mpu6050_DATA_OUT;
	return tem;  
}

u8 mpu6050_Read(void)
{
	u8 read,j;
	read=0x00;
	
	mpu6050_DATA_IN;
	delayus(2);
	for(j=8;j>0;j--)
	{		     
		delayus(2);
		mpu6050_SIC_H();
		delayus(2);
		read=read<<1;
		if(mpu6050_SID_STATE)
			read=read+1;  
		mpu6050_SIC_L();
		delayus(2);
	}	
    mpu6050_DATA_OUT;
	return read;
}

u8 WriteReg(u8 a,u8 regID, u8 regDat)
{
	mpu6050_Start();
	if(mpu6050_Write(a)==0)
	{
		mpu6050_Stop();
		return 1;
	}
	delayus(2);
  	if(mpu6050_Write(regID)==0)
	{
		mpu6050_Stop();
		return 2;
	}
	
	delayus(2);
  	if(mpu6050_Write(regDat)==0)
	{
		mpu6050_Stop();
		return 3;
	}
  	mpu6050_Stop();
  	return 0;
}


u8 ReadReg(u8 a,u8 regID)
{
	u8 regDat;
	
	mpu6050_Start();
	if(mpu6050_Write(a&0xfe)==0)
	{
		mpu6050_Stop();
		return 1;
	}
	delayus(2);
  	if(mpu6050_Write(regID)==0)
	{
		mpu6050_Stop();
		return 2;
	}
	delayus(2);

	mpu6050_Stop();
	delayus(2);	
	
	mpu6050_Start();
	
	if(mpu6050_Write(a+1)==0)
	{
		mpu6050_Stop();
		return 3;
	}

  	regDat=mpu6050_Read();
	noAck();
  	mpu6050_Stop();
  	return regDat;
}
u8 InitMPU6050()
{
	 mpu6050_Init();
	 if(WriteReg(SlaveAddress,PWR_MGMT_1, 0x01)) 
		 return 1;
	 WriteReg(SlaveAddress,SMPLRT_DIV, 0x04);
	 WriteReg(SlaveAddress,MPUCONFIG, 0x02);
	 WriteReg(SlaveAddress,GYRO_CONFIG, 3<<3);
	 WriteReg(SlaveAddress,ACCEL_CONFIG, 2<<3);
	 WriteReg(SlaveAddress,INT_PIN_CFG, 0x32);
	 WriteReg(SlaveAddress,INT_ENABLE, 0x01);
	 WriteReg(SlaveAddress,USER_CTRL, 0x00);
	return 0;
}
void mpu_get_data()
{
	mpu_data[1]=ReadReg(SlaveAddress,ACCEL_XOUT_H);
	mpu_data[0]=ReadReg(SlaveAddress,ACCEL_XOUT_L);
	mpu_data[3]=ReadReg(SlaveAddress,ACCEL_YOUT_H);
	mpu_data[2]=ReadReg(SlaveAddress,ACCEL_YOUT_L);
	mpu_data[5]=ReadReg(SlaveAddress,ACCEL_ZOUT_H);
	mpu_data[4]=ReadReg(SlaveAddress,ACCEL_ZOUT_L);
	mpu_data[7]=ReadReg(SlaveAddress,TEMP_OUT_H);
	mpu_data[6]=ReadReg(SlaveAddress,TEMP_OUT_L);
	mpu_data[9]=ReadReg(SlaveAddress,GYRO_XOUT_H);
	mpu_data[8]=ReadReg(SlaveAddress,GYRO_XOUT_L);
	mpu_data[11]=ReadReg(SlaveAddress,GYRO_YOUT_H);
	mpu_data[10]=ReadReg(SlaveAddress,GYRO_YOUT_L);
	mpu_data[13]=ReadReg(SlaveAddress,GYRO_ZOUT_H);
	mpu_data[12]=ReadReg(SlaveAddress,GYRO_ZOUT_L);
}

