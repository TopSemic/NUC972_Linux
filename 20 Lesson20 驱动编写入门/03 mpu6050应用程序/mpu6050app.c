#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

short x_accel, y_accel, z_accel;
short x_gyro, y_gyro, z_gyro;
short temp;

int main()
{
    char buffer[128];
    short *value;
    int in, out;
    int nread;
    
    in = open("/dev/mpu6050", O_RDONLY);
    if (!in) {
        printf("ERROR: %d, Open /dev/mpu6050 failed.\n", -1);
        return -1;    
    }    
    nread = read(in, buffer, 12);
    close(in);    
    if (nread < 0) {
        printf("ERROR: %d, A read error has occurred\n", nread);
        return -1;    
    }

    value = (short*)buffer;
    x_accel = *(value);
    y_accel = *(value + 1);
    z_accel = *(value + 2);
	temp    = *(value + 3);
    x_gyro =  *(value + 4);
    y_gyro =  *(value + 5);
    z_gyro =  *(value + 6);
    printf("x accel is: %d \n", x_accel);
    printf("y accel is: %d \n", y_accel);
    printf("z accel is: %d \n", z_accel);    
    printf("x gyro is: %d \n", x_gyro);
    printf("y gyro is: %d \n", y_gyro);
    printf("z gyro is: %d \n", z_gyro);
	printf("temperature is: %d \n", temp);

    exit(0);
}