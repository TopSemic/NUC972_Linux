GCC=arm-linux-gcc -march=armv5te -ljpeg
SUBDIRS = main \
	image_process \
	v4l2 \
	lcd \
	obj \
		
		
BIN = mainapp
OBJS = main.o v4l2_use.o lcd.o image_process.o
OBJS_DIR = obj
BIN_DIR = bin
CFLAG = -I../include 

export GCC OBJS OBJS_DIR BIN_DIR BIN  CFLAG   
	
all:CHECK_DIR $(SUBDIRS)
CHECK_DIR:
		mkdir -p $(BIN_DIR)
$(SUBDIRS) : ECHO
		make -C $@ 

ECHO:
	@echo $(SUBDIRS)
	@echo begin compile
c:
	@$(RM) $(OBJS_DIR)/*.o	
	@rm -rf $(BIN_DIR)
	@echo rm -rf $(OBJS_DIR)/*.o	

