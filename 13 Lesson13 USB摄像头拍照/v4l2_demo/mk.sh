#! /bin/sh  运行脚本：sh mk.sh 

	rm /tftpboot/rootfs/project/day8/mainapp
	cp bin/mainapp /tftpboot/rootfs/project/day8
	echo  cp bin/mainapp /tftpboot/rootfs/project/day8
#	modprobe $modname
#	mknod /dev/$devname c 250 0
#	echo compied !
