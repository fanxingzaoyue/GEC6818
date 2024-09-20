#ifndef V4L2_H
#define V4L2_H
struct Frame{
	char *addr;	//保存映射得到帧缓冲区的首地址
	int len;	//帧缓冲的大小
	int width;	//宽
	int height; //高
	int index;	//索引
};
void v4l2_init();
void v4l2_open_device(const char* devname);
void v4l2_set_format(int width, int height);
void v4l2_get_format(int index);
void v4l2_request_buffers();
void v4l2_mmap_enqueue();
void v4l2_enqueue(int index);
int v4l2_dequeue();
void v4l2_start_capture();
struct Frame v4l2_get_frame();
void v4l2_print_addrs();
void v4l2_uninit();

#endif



