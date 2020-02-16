obj-m := hello.o
PWD := $(shell pwd)
KDIR :=/home/topsemic/nuc972/kernel/NUC970_Linux_Kernel-master/

all:
        $(MAKE) -C $(KDIR) M=$(PWD)
clean:
        rm -rf *.o *.mod.c *.mod.o *.ko *.symvers *.order *.a
