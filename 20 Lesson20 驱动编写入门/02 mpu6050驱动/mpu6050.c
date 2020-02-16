#include"mpu6050bsp.h"

int MPU6050_MAJOR = 0;
int MPU6050_MINOR = 0;
int NUMBER_OF_DEVICES = 2;
 
struct class *my_class;
struct cdev cdev;
dev_t devno;
/*************************************************************************************/ 


#define DRIVER_NAME "mpu6050"

int mpu6050_open(struct inode *inode,struct file *filp)
{
	u8 reg;
	reg=InitMPU6050();

	printk("mpu6050:%d\n",reg);
	return nonseekable_open(inode,filp);
}

long mpu6050_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
		
		default:
			return -2;
	}
	return 0;
}
int mpu6050_read(struct file *filp, char *buffer,size_t count, loff_t *ppos)
{
	mpu_get_data();
	return copy_to_user(buffer, mpu_data, 14);
}
int mpu6050_write(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{	
	return 0;
}

struct file_operations mpu6050_fops = {
	.owner = THIS_MODULE,
	.read = mpu6050_read,
	.write = mpu6050_write,
	.open 	= mpu6050_open,
	.unlocked_ioctl = mpu6050_ioctl,
};


/**************************************************************************************/
static int __init mpu6050_init(void)
{
    int result;
    devno = MKDEV(MPU6050_MAJOR, MPU6050_MINOR);
    if (MPU6050_MAJOR)
        result = register_chrdev_region(devno, 2, "mpu6050");
    else
    {
        result = alloc_chrdev_region(&devno, 0, 2, "mpu6050");
        MPU6050_MAJOR = MAJOR(devno);
    }  
    printk("MAJOR IS %d\n",MPU6050_MAJOR);
    my_class = class_create(THIS_MODULE,"mpu6050_class");  //类名为
    if(IS_ERR(my_class)) 
    {
        printk("Err: failed in creating class.\n");
        return -1; 
    }
    device_create(my_class,NULL,devno,NULL,"mpu6050");      //设备名为mpu6050
    if (result<0) 
    {
        printk (KERN_WARNING "hello: can't get major number %d\n", MPU6050_MAJOR);
        return result;
    }
 
    cdev_init(&cdev, &mpu6050_fops);
    cdev.owner = THIS_MODULE;
    cdev_add(&cdev, devno, NUMBER_OF_DEVICES);
    printk (KERN_INFO "mpu6050 driver Registered\n");
    return 0;
}
 
static void __exit mpu6050_exit  (void)
{
    cdev_del (&cdev);
    device_destroy(my_class, devno);         //delete device node under /dev//必须先删除设备，再删除class类
    class_destroy(my_class);                 //delete class created by us
    unregister_chrdev_region (devno,NUMBER_OF_DEVICES);
    printk (KERN_INFO "char driver cleaned up\n");
}
 
module_init (mpu6050_init );
module_exit (mpu6050_exit );
 
MODULE_LICENSE ("GPL");
