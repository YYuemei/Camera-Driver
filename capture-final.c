<<<<<<< HEAD
=======
/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 */

>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

struct buffer {
<<<<<<< HEAD
	unsigned char *         start;
	size_t                  length;
};

char *           dev_name        = NULL;
int              fd              = -1;
struct buffer *         buffers         = NULL;
unsigned int     n_buffers       = 0;

//FILE *file_fd;

//#define _DEBUG 1

void errno_exit(const char *s)
{
	printf ( "%s error %d\n",
			s, errno);
=======
	unsigned char *                  start;
	size_t                  length;
};

static char *           dev_name        = NULL;
static int              fd              = -1;
struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;

FILE *file_fd;

//#define _DEBUG 1

static void errno_exit(const char *s)
{
	fprintf (stderr, "%s error %d, %s\n",
			s, errno, strerror (errno));
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf

	exit (EXIT_FAILURE);
}

<<<<<<< HEAD
int xioctl (int fd,int request,void *arg)
=======
static int xioctl (int fd,int request,void *arg)
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
{
	int r;

	do r = ioctl (fd, request, arg);
	while (-1 == r && EINTR == errno);

	return r;
}


<<<<<<< HEAD
int read_frame(void)
=======
static int read_frame(void)
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
{
	struct v4l2_buffer buf;
	unsigned int i;

	CLEAR (buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {       //出列 采集的帧缓冲

		errno_exit ("VIDIOC_DQBUF");
	}


	assert (buf.index < n_buffers);           //测试一个条件并可能使程序终止

<<<<<<< HEAD
	//fwrite( buffers[buf.index].start, buffers[buf.index].length, 1,file_fd);
////////////////////////////////////////////////////////////////////////////////////
	int light = 0;
	xioctl(fd,VIDIOC_G_STD,&light);
	printf("%d\n",light);
	light= light/2+80;
	if(light>255)
		light = 255;
	char cmd[27]= {"setpci -s 00:02.0 F4.B="};
	char temp[2 ];
	int t,q ;
	t = light/16;
	q = light%16; 
	//printf("%s\n",cmd);
	sprintf(temp,"%X",t);
	cmd[23] = temp[0];
	//printf("%s\n",temp);
	//printf("%s\n",cmd);
	sprintf(temp,"%X",q);
	cmd[24] = temp[0];	
	//cmd[25] = "\0";
	printf("%s\n",cmd);
	system(cmd);
////////////////////////////////////////////////////////////////////////////////////////
=======
	fwrite( buffers[buf.index].start, buffers[buf.index].length, 1,file_fd);
	int light = 0;
	xioctl(fd,VIDIOC_G_STD,&light);
	printf("%d\n",light);
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
	int ff,m= 0;
	for(ff= 0;ff<buffers[buf.index].length;ff++)
	{
		int k = buffers[buf.index].start[ff];
		if(ff%1024==0)
		{
			m=m+k;
		//	printf("%d-",k);
		}

	//printf("%d\n",m);
	}
	printf("light:%d\n",m/(ff/2048));
<<<<<<< HEAD
	//printf("start:%p,lenth:%d\n",buffers[buf.index].start,buffers[buf.index].length);
	if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))       // 再次，将 缓冲区 入列
		errno_exit ("VIDIOC_QBUF");

	return 1;
}

void mainloop(void)
=======
	printf("start:%p,lenth:%d\n",buffers[buf.index].start,buffers[buf.index].length);
	if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))       // 再次，将 缓冲区 入列
		errno_exit ("VIDIOC_QBUF");




	return 1;
}

static void mainloop(void)
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
{
	for (;;) 
	{
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);
		FD_SET (fd, &fds);

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select (fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) 
		{
			if (EINTR == errno)
				continue;
			errno_exit ("select");
		}

		if (0 == r) 
		{
<<<<<<< HEAD
			printf ( "select timeout\n");
=======
			fprintf (stderr, "select timeout\n");
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
			exit (EXIT_FAILURE);
		}

		if (read_frame ())
			break;
	}
}

<<<<<<< HEAD
void stop_capturing(void)
=======
static void stop_capturing(void)
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
{

}

<<<<<<< HEAD
void start_capturing(void)
=======
static void start_capturing(void)
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
{
	unsigned int i;
	enum v4l2_buf_type type;

	for (i = 0; i < n_buffers; ++i) 
	{
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;

		if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
			errno_exit ("VIDIOC_QBUF");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
		errno_exit ("VIDIOC_STREAMON");
}

static void uninit_device(void)
{
	unsigned int i;

	for (i = 0; i < n_buffers; ++i)
		if (-1 == munmap (buffers[i].start, buffers[i].length))
			errno_exit ("munmap");

	free (buffers);
}


static void init_mmap(void)
{
	struct v4l2_requestbuffers req;

	CLEAR (req);
	req.count		=1;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req)) 
	{
		if (EINVAL == errno) 
		{
<<<<<<< HEAD
			printf ( "%s does not support "
=======
			fprintf (stderr, "%s does not support "
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
					"memory mapping\n", dev_name);
			exit (EXIT_FAILURE);
		} 
		else 
		{
			errno_exit ("VIDIOC_REQBUFS");
		}
	}

<<<<<<< HEAD
#ifdef _DEBUG_
=======
//#ifdef _DEBUG_
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
	printf("count				:%d\n\n",req.count);
	printf("type(CAPTURE=1)			:%d\n",req.type);
	printf("memory(MEMORY_MMAP=1)		:%d\n\n",req.memory);

<<<<<<< HEAD
#endif
=======
//#endif
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf


	if (req.count < 1) 
	{
<<<<<<< HEAD
		printf ( "Insufficient buffer memory on %s\n",
=======
		fprintf (stderr, "Insufficient buffer memory on %s\n",
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
				dev_name);
		exit (EXIT_FAILURE);
	}

	buffers = calloc (req.count, sizeof (*buffers));  //用户空间，内存中建立 对应空间

	if (!buffers) 
	{
<<<<<<< HEAD
		printf ( "Out of memory\n");
=======
		fprintf (stderr, "Out of memory\n");
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
		exit (EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) 
	{
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;

		if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
			errno_exit ("VIDIOC_QUERYBUF");



<<<<<<< HEAD
#ifdef _DEBUG
=======
//#ifdef _DEBUG
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
		printf("index                        :%d\n",buf.index);
		printf("buf_type(CAPTURE=1)          :%d\n",buf.type);
		printf("bytesused                    :%d\n",buf.bytesused);
		printf("flags                        :%d\n",buf.flags);	
		printf("filed(NONE=1)                :%d\n",buf.field);

		printf("sequence                     :%d\n",buf.sequence);
		printf("memory(MMAP=1)               :%d\n",buf.memory);
		printf("m.offset                     :%d\n",buf.m.offset);
		printf("length                       :%d\n\n",buf.length);
		printf("input                        :%d\n\n",buf.input);
<<<<<<< HEAD
#endif
=======
//#endif
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =
			mmap (NULL /* start anywhere */,
					buf.length,
					PROT_READ | PROT_WRITE /* required */,
					MAP_SHARED /* recommended */,
					fd, buf.m.offset);  //fd="/dev/video0"

		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit ("mmap");
	}
}


static void init_device(void)
{
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	unsigned int min;

	if (-1 == xioctl (fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
<<<<<<< HEAD
			printf ("%s is no V4L2 device\n",
=======
			fprintf (stderr, "%s is no V4L2 device\n",
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
					dev_name);
			exit (EXIT_FAILURE);
		} else {
			errno_exit ("VIDIOC_QUERYCAP");
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
<<<<<<< HEAD
		printf ( "%s is no video capture device\n",
				dev_name);
		exit (EXIT_FAILURE);
	}
#ifdef _DEBUG
=======
		fprintf (stderr, "%s is no video capture device\n",
				dev_name);
		exit (EXIT_FAILURE);
	}
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
	printf("drivers     :%s\n",cap.driver);
	printf("card        :%s\n",cap.card);
	printf("bus_info    :%s\n",cap.bus_info);
	printf("16-jinzhi capabilities:%x\n",cap.capabilities);
	printf("10-jinzhi capabilities:%d\n\n",cap.capabilities);
<<<<<<< HEAD
#endif
=======

>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
	CLEAR (fmt);

	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
		errno_exit ("VIDIOC_S_FMT");


	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;

#ifdef _DEBUG
	printf("v4l2_buf_type(CAPTURE=1)	:%d\n",fmt.type);  /* CAPTURE=1*/
	printf("width      		  	:%d\n",fmt.fmt.pix.width);
	printf("height       			:%d\n",fmt.fmt.pix.height);
	printf("pixel'yuyv'    			:%d\n",('Y'|'U'<<8|'Y'<<16|'V'<<24));
	printf("pixel'mjpg'    		 :%d\n",('M'|'J'<<8|'P'<<16|'G'<<24));
	printf("pixel'jpeg'     		:%d\n",('J'|'P'<<8|'E'<<16|'G'<<24));
<<<<<<< HEAD

=======
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
	printf("pixelformat 		 	:%d\n",fmt.fmt.pix.pixelformat);
	printf("v4l1_filed(FIELD_INTERCACED=4)  :%d\n",fmt.fmt.pix.field);
	printf("bytesperline		        :%d\n",fmt.fmt.pix.bytesperline);
	printf("sizeimage		        :%d\n",fmt.fmt.pix.sizeimage);
	printf("colorspace(COLOSPACE_JPEG=7)	:%d\n",fmt.fmt.pix.colorspace);
	printf("priv			        :%d\n\n",fmt.fmt.pix.priv);
#endif

	init_mmap ();

}

<<<<<<< HEAD
void close_device(void)
=======
static void close_device(void)
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
{
	if (-1 == close (fd))
		errno_exit ("close");

	fd = -1;
}

<<<<<<< HEAD
void open_device(void)
{
	fd = open (dev_name, O_RDWR);

	if (-1 == fd) {
		printf ( "Cannot open '%s': %d\n",
				dev_name, errno);
=======
static void open_device(void)
{
	fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

	if (-1 == fd) {
		fprintf (stderr, "Cannot open '%s': %d, %s\n",
				dev_name, errno, strerror (errno));
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
		exit (EXIT_FAILURE);
	}
}

int main(int argc,char **argv)
{
<<<<<<< HEAD
//for(;;){
	//file_fd=fopen("mmap.jpg","w");    
	dev_name = "/dev/video0";

	open_device ();
//	printf("I'm 1~\n");
	init_device ();
//	sleep(10);
//	printf("I'm 2~\n");
	start_capturing ();
//	printf("I'm 3~\n");
=======
	file_fd=fopen("mmap.jpg","w");    
	dev_name = "/dev/video0";

	open_device ();
	printf("I'm 1~\n");
	init_device ();
//	sleep(10);
	printf("I'm 2~\n");
	start_capturing ();
	printf("I'm 3~\n");
>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
	mainloop ();

	stop_capturing ();

<<<<<<< HEAD

	uninit_device ();

	close_device ();
//sleep(10);
//}
=======
	uninit_device ();

	close_device ();

>>>>>>> ad10c78009517700cc08889c7239005a1336c1bf
	exit (EXIT_SUCCESS);

	return 0;
}
