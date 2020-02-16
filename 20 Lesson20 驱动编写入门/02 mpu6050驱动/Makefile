MODULE_NAME := mpu6050d
RESMAN_CORE_OBJS:=mpu6050.o
RESMAN_GLUE_OBJS:=mpu6050bsp.o
$(MODULE_NAME)-objs:=$(RESMAN_GLUE_OBJS) $(RESMAN_CORE_OBJS)
obj-m :=mpu6050d.o

PWD  := $(shell pwd)
KVER ?= $(shell uname -r)

KDIR :=/home/topsemic/nuc972/kernel/NUC970_Linux_Kernel-master/
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions

