#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>           

#include <fcntl.h>            
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>        
#include <linux/videodev2.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

struct buffer {
	void *                  start;
	size_t                  length;
};

static char *           dev_name        = "/dev/video0";//摄像头设备名
static int              fd              = -1;
struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;

FILE *file_fd;
static unsigned long file_length;
static unsigned char *file_name;
//////////////////////////////////////////////////////
//获取一帧数据
//////////////////////////////////////////////////////
static int read_frame (void)
{
	struct v4l2_buffer buf;
	unsigned int i;

	CLEAR (buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	ioctl (fd, VIDIOC_DQBUF, &buf); //出列采集的帧缓冲

	assert (buf.index < n_buffers);
	printf ("buf.index dq is %d\n",buf.index);

	fwrite(buffers[buf.index].start, buffers[buf.index].length, 1, file_fd); //将其写入文件中

	ioctl (fd, VIDIOC_QBUF, &buf); //再将其入列

	return 1;
}

int main (int argc,char ** argv)
{
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	unsigned int i;
	enum v4l2_buf_type type;

	file_fd = fopen("test-mmap.jpeg", "w");//图片文件名

	fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);//打开设备

	ioctl (fd, VIDIOC_QUERYCAP, &cap);//获取摄像头参数

	CLEAR (fmt);
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	ioctl (fd, VIDIOC_S_FMT, &fmt); //设置图像格式

	file_length = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; //计算图片大小

	struct v4l2_requestbuffers req;
	CLEAR (req);
	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	ioctl (fd, VIDIOC_REQBUFS, &req); //申请缓冲，count是申请的数量

	if (req.count < 2)
		printf("Insufficient buffer memory\n");

	buffers = calloc (req.count, sizeof (*buffers));//内存中建立对应空间

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
	{
		struct v4l2_buffer buf;   //驱动中的一帧
		CLEAR (buf);
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;

		if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) //映射用户空间
			printf ("VIDIOC_QUERYBUF error\n");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =
			mmap (NULL /* start anywhere */,    //通过mmap建立映射关系
					buf.length,
					PROT_READ | PROT_WRITE /* required */,
					MAP_SHARED /* recommended */,
					fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start)
			printf ("mmap failed\n");
	}

	for (i = 0; i < n_buffers; ++i)
	{
		struct v4l2_buffer buf;
		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;

		if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))//申请到的缓冲进入列队
			printf ("VIDIOC_QBUF failed\n");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
		printf ("VIDIOC_STREAMON failed\n");

	for (;;) //这一段涉及到异步IO
	{
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);//将指定的文件描述符集清空
		FD_SET (fd, &fds);//在文件描述符集合中增加一个新的文件描述符

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select (fd + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时

		if (-1 == r) {
			if (EINTR == errno)
				continue;
			printf ("select err\n");
		}
		if (0 == r) {
			fprintf (stderr, "select timeout\n");
			exit (EXIT_FAILURE);
		}

		if (read_frame ())//如果可读，执行read_frame ()函数，并跳出循环
			break;
	}

unmap:
	for (i = 0; i < n_buffers; ++i)
		if (-1 == munmap (buffers[i].start, buffers[i].length))
			printf ("munmap error");
	close (fd);
	fclose (file_fd);
	exit (EXIT_SUCCESS);
	return 0;
}///////////////////////////////////////
