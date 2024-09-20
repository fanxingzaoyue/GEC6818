#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cmath>
#include <unistd.h>
#include <linux/input.h>

#include "touch.h"
using namespace std;
const int MAX_x = 800,Max_y = 480,MAX_rx = 1020,MAX_ry = 600;
point Turn_real(point pre){
	point ans{};
	ans.x = pre.x * MAX_x / MAX_rx ;
	ans.y = pre.y * Max_y / MAX_ry ;
	return  ans;
}
point get_touch() {
	int fd = open ("/dev/input/event0", O_RDWR);

	if (fd == -1) {
		perror("Fail to open input/event0");
	}

	int x = -1, y = -1;
	struct input_event	ev;

	while (true) {
		read(fd , &ev, sizeof(ev));    // --> struct input_event{} 中
		//解析坐标
		if ( ev.type == EV_ABS ) {	//触摸事件
			if ( ev.code == ABS_X )	{ //x轴
				x = ev.value;
			}
			else if ( ev.code == ABS_Y ) {//y轴
				y = ev.value;
			}
			else if ( ev.code == ABS_PRESSURE && ev.value == 0) {	//压力事件 压力为0 就结束循环
				break;
				//退出
			}
		}
		else if ( ev.type == EV_KEY && ev.value == 0) {	//按键事件 ，按键松开，就结束循环
			break;
			//退出
		}
	}
	//打印
	printf("press x= %d y= %d\n", x, y);
	//关闭屏幕
	close(fd);
	point ans{};
	ans.x = x; ans.y = y;
	return ans;
}

int get_direction() {
	//打开屏幕
	int fd = open ("/dev/input/event0", O_RDWR);
	if (fd == -1) {
		perror("Fail to open input/event0");
		return -1;
	}
	//读取数据
	int x0 = -1, y0 = -1;   //起点
	int x1 = -1, y1 = -1;   //终点
	struct input_event	ev;
	while (true) {
		read(fd , &ev, sizeof(ev));
		if (ev.type == EV_ABS) {

			if (ev.code == ABS_X)   { //x轴
				if ( x0 == -1 ) {
					x0 = ev.value;
				}
				x1 = ev.value;
			}
			else if (ev.code == ABS_Y)  { //y轴
				if ( y0 == -1 ) {
					y0 = ev.value;
				}
				y1 = ev.value;
			} else if (ev.code == ABS_PRESSURE && ev.value == 0) {
				break;
			}
		} else if ( ev.type == EV_KEY && ev.value == 0) {	//按键事件 ，按键松开，就结束循环
			break;
			//退出
		}
	}

	//关闭屏幕
	close(fd);
	printf("x0 = %d x1 = %d\n", x0, x1);
	printf("y0 = %d y1 = %d\n", y0, y1);

	// 判断方向
	if (abs(y0 - y1) > abs(x1 - x0) ) {
		if (y1 < y0) {
			if (y0 > 480) return QUIT;
			return UP;
		}
		return DOWN;
	}
	if (x1 < x0) return LEFT;
	return RIGHT;
}

