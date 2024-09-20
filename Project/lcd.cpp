#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <sys/mman.h>
#include <cmath>
#include "lcd.h"
/*
addr：地址，指定要映射到内存的哪个地址上去
一般填null ，表示让系统自行分配
length： 指定要映射多大空间，单位字节
prot： 指定映射区域的权限
PROT_EXEC 可执行
PROT_READ 可读
PROT_WRITE 可写
PROT_NONE 无权限
读写
PROT_READ | PROT_WRITE
flags:映射标志
MAP_SHARED 共享映射，对映射区的操作会立即反馈到文件中
MAP_PRIVATE 私有映射，对映射区的操作仅对代码可见
fd:指定要映射的文件的描述符
offset:偏移量
 */
int fd = -1; // 屏幕文件的描述符
int *plcd;
int lcd_init() {
	// 打开屏幕
	fd = open("/dev/fb0", O_RDWR);
	if (fd == -1) {
		perror("Fail to open file /dev/fb0\n");
		return -1;
	} 
	//内存映射
	plcd = (int *)mmap(NULL, 480 * 800 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (plcd == MAP_FAILED) {
		perror("Fail to mmap file /dev/fb0.\n");
		return -1;
	}
	return 1;
}
// 关闭屏幕
void lcd_close() {
	// 解除映射
	int op = munmap(plcd, 480 * 800 * 4);
	if (op == -1) {
		perror("Fail to unmap file\n");
	}
	// 关闭屏幕
	op = close(fd);
	if (op == -1) {
		perror("Fail to close file.\n");
	} 
}
void display_point(int x, int y, int color) {
	if ( x >= 0 && x < 800 && y >= 0 && y < 480 ) {
		*(plcd + y * 800 + x) = color;
	}
}
// 显示一张纯色的图片（清屏）
void show_a_color(int color) {
	for (int i = 0; i < 800; i++) {
		for (int j = 0; j < 480; j++) {
			display_point(i, j, color);
		}
	}
}
// 画矩形
void display_Rectangle(int x0, int y0, int w, int h, int color) {
	for (int i = x0; i < x0 + w; i++) {
		for (int j = y0; j < y0 + h; j++) {
			display_point(i, j, color);
		}
	}
}
// 画圆
void display_Circle(int x0, int y0, int r, int color) {
	// (x-x0)^2 + (y - y0)^2 = r*r
	for (int i = 0; i < 800; i++) {
		for (int j = 0; j < 480; j++) {
			if ( (i - x0) * (i - x0) + (j - y0) * (j - y0) <= r * r ) {
				display_point(i, j, color);
			}
		}
	}
}
// 画三角形
void display_Tangle(int x0, int y0, int h, int color) {
	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 480; y++) {
			if ( y >= y0 && y <= y0 + h && y >= x0 - x + y0 && y >= x - x0 + y0) {
				display_point(x, y, color);
			}
		}
	}
}