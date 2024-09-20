#ifndef _TOUCH_H_
#define _TOUCH_H_
#define UP  1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define	QUIT 5
#define BLUE   0x000000FF   // 纯白色16进制表示aRGB
#define RED    0x00FF0000   // 纯红色16进制表示aRGB
#define GREEN  0x0000FF00   // 纯蓝色16进制表示aRGB
#define WHITE  0x00FFFFFF   // 白色16进制表示aRGB
#define YELLOW 0x00FFFF00   // 黄色16进制表示aRGB
#define BLACK  0x00000000   // 黑色16进制表示aRGB
struct point {
	int x;
	int y;
};
point get_touch() ;
int get_direction();
point Turn_real(point pre);
#endif
