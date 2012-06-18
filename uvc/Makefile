obj-m := uvcvideo.o 
uvcvideo-objs := uvc_driver.o uvc_queue.o uvc_v4l2.o uvc_video.o uvc_ctrl.o \
		  uvc_status.o 

KERNELBUILD := /lib/modules/`uname -r`/build
default:
	 make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	 rm -rf *.o .*.cmd *.ko *.mod.c .tmp_versions
