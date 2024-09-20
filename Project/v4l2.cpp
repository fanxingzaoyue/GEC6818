//
// Created by 刘云志· on 2024/9/11.
//
#include "v4l2.h"
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/videodev2.h>



#define NBUFFS 5

static Frame frames[NBUFFS] = {0};
static int fd = -1;


void v4l2_init()
{
	//打开摄像头
	v4l2_open_device("/dev/video7");

	//设置格式,显示在桌面大小
	v4l2_set_format(640, 480);

	//申请缓冲区
	v4l2_request_buffers();

	//映射
	v4l2_mmap_enqueue();

	//打印 映射后的地址
	v4l2_print_addrs();

	//启动
	v4l2_start_capture();

	printf("video init success!!\n");
}

void v4l2_open_device(const char* devname)
{
	fd = open(devname, O_RDWR);
	if (fd == -1)
	{
		perror("open video error");
		exit(1);
	}
	printf("open video ok!!\n");
}

void v4l2_set_format(int width, int height)
{
	//检查宽高是否合法
	if (!((width==160 || width==320 || width==640) && (height==120 || height==240 || height==480)))
	{
		printf("width or height invalid!!\n");
		exit(1);
	}

	struct v4l2_format format;
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.width = width;
	format.fmt.pix.height = height;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;//可选
	printf("fd = %d\n", fd);
	int res = ioctl(fd, VIDIOC_S_FMT, &format);
	if (res == -1)
	{
		perror("set format error");
		exit(1);
	}

	printf("set format OK!!\n");
}

void v4l2_get_format(int index)
{
	struct v4l2_format format;
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int res = ioctl(fd, VIDIOC_G_FMT, &format);
	if (res == -1)
	{
		perror("get format error");
		exit(1);
	}
	frames[index].width = format.fmt.pix.width;
	frames[index].height = format.fmt.pix.height;
}

void v4l2_request_buffers()
{
	struct v4l2_requestbuffers reqbuffers;
	reqbuffers.count = NBUFFS;
	reqbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuffers.memory = V4L2_MEMORY_MMAP;
	int res = ioctl(fd, VIDIOC_REQBUFS, &reqbuffers);
	if (res == -1)
	{
		perror("request buffer error");
		exit(1);
	}
	printf("request buffer success!!\n");
}

void v4l2_start_capture()
{
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int res = ioctl(fd, VIDIOC_STREAMON, &type);
	if (res == -1)
	{
		perror("stream on error");
		exit(1);
	}
}

void v4l2_print_addrs()
{
	//测试应用空间的帧缓冲的地址和长度
	int i;
	for(i = 0; i < NBUFFS; ++i)
	{
		printf("addr: %p, len: %d\n", frames[i].addr, frames[i].len);
	}
}

void v4l2_mmap_enqueue()
{
	int i;
	int res;
	for (i = 0; i < NBUFFS; ++i)
	{
		struct v4l2_buffer buffer;
		buffer.index = i;
		buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		//查询
		res = ioctl(fd, VIDIOC_QUERYBUF, &buffer);
		if (res == -1)
		{
			perror("query buffer error");
			exit(1);
		}
		//映射
		frames[i].index = i;
		frames[i].len = buffer.length;
		frames[i].addr = (char *)mmap(NULL, buffer.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, buffer.m.offset);
		if (frames[i].addr == MAP_FAILED)
		{
			perror("mmap error");
			exit(1);
		}
		//入队
		v4l2_enqueue(i);
	}
}

void v4l2_enqueue(int index)
{
	struct v4l2_buffer buffer;
	buffer.index = index;
	buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	//查询
	int res = ioctl(fd, VIDIOC_QUERYBUF, &buffer);
	if (res == -1)
	{
		perror("query buffer error");
		exit(1);
	}
	//入队
	res = ioctl(fd, VIDIOC_QBUF, &buffer);
	if (res == -1)
	{
		perror("add queue error");
		exit(1);
	}
	// printf("%d buffer enqueue success!!\n", index);
}

int v4l2_dequeue()
{
	struct v4l2_buffer buffer;
	buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int res = ioctl(fd, VIDIOC_DQBUF, &buffer);
	if (res == -1)
	{
		perror("dequeue error");
		exit(1);
	}
	// printf("dequeue addr: %x, len = %d, \n", frames[buffer.index].addr, frames[buffer.index].len);
	return buffer.index;
}

struct Frame v4l2_get_frame()
{
	int index = v4l2_dequeue();
	v4l2_get_format(index);
	return frames[index];
}

void v4l2_uninit()
{
	int i;
	for (i = 0; i < NBUFFS; ++i)
	{
		munmap(frames[i].addr, frames[i].len);
	}
	close(fd);
	printf("v4l2_uninit!\n");
}
