#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <sys/mman.h>
#include <cmath>
#include "lcd.h"
#include "touch.h"
#include "bmp.h"
#include "music.h"
#include "photo.h"
#include <string>
#include <linux/input.h>
#include "video.h"
#include <thread>
#include "common.h"
#include <sys/ioctl.h>
#include "v4use.h"
#define REC_CMD  "./arecord -d4 -c1 -r16000 -traw -fS16_LE cmd.pcm"
#define REC_CMD  "arecord -d3 -c1 -r16000 -traw -fS16_LE cmd.pcm"
#define PCM_FILE "./cmd.pcm"
/* -d：录音时长（duration）
-c：音轨（channels）
-r：采样频率（rate）
-t：封装格式（type）
-f：量化位数（format） */
using namespace std;
void Catch (int sig) {
	if (sig == SIGPIPE)
	{
		printf("killed by SIGPIPE\n");
		exit(0);
	}
}
const char * s = "Bmp/main.bmp";
char Main_page[5][30] = {
	"Bmp/main.bmp","Bmp/Music.bmp"
};
void Touch_Control() {
	while (true) {
		if (Stop == 0) { //当前处于主界面时，可以触碰调用函数
			point g = Turn_real(get_touch()) ;
			int x = g.x,y = g.y;
			if ( x>= 123 && x <= 214 && y >= 324 && y <= 423) {
				show_photo();
			} else if (x>= 351 && x <= 468 && y >= 325 && y <= 435&&Stop==0) {
				control_video (&Stop);
			} else if (x>= 564 && x <= 676 && y >= 307 && y <= 431&&Stop==0) {
				control_music(&Stop);
			}
		}
	}
}

void LED(int state) {
	unsigned char cmd[2];
	int fd_led=open("/dev/led_drv",O_RDWR);
	if(fd_led==-1)perror("error");
	for (int i=0;i<3;i++) {
		cmd[0]=state;
		cmd[1]= 8 + i;
		write(fd_led,&cmd,2);
	}
	close(fd_led);
}

void pwm(int time) {
	int fd_pwm=open("/dev/pwm",O_RDWR);
	if(fd_pwm==-1)perror("error");
	unsigned char cmd=1;
	write(fd_pwm,&cmd,1);
	sleep(time);
	cmd=0;
	write(fd_pwm,&cmd,1);
	close(fd_pwm);
}

//static  int stop=0;
int main(int argc, char const* argv[]) {
	//signal(SIGPIPE, Catch);
	lcd_init();
	Stop = 0;
	thread touch(Touch_Control);
	touch.detach();
	show_bmp(0,0,s);
	// 语音识别
	/*
	 */
	if (argc != 2)
	{
		printf("Usage: %s <ubuntu-IP>\n", argv[0]);
		exit(0);
	}
	// 初始化TCP客户端套接字，用于连接到语音识别服务器(即ubuntu)
	int sockfd_tcp = init_tcp_socket(argv[1]);
	// 初始化本地UDP套接字
	int sockfd_udp = init_udp_socket();

	int id_num = -1; // 识别后的指令id
	while (true) {
		// 1，调用arecord来录一段音频
		printf("please to start REC in 3s...\n");
		// 在程序中执行一条命令  “录音的命令”
		system(REC_CMD);
		// 2，将录制好的PCM音频发送给语音识别引擎
		send_pcm(sockfd_tcp, PCM_FILE);
		// 3，等待对方回送识别结果（字符串ID）
		xmlChar* id = wait4id(sockfd_tcp);
		if (id == NULL)	{
			continue;
		}
		id_num = atoi( (char*)id);

		switch (id_num) {
			case 1:  // 相册
				show_photo();
				break;
			case 2:  // 视频
				control_video(&Stop);
				break;
			case 3: // 音乐
				control_music(&Stop);
				break;
			case 4: // LED 开
				LED(1);
				break;
			case 5: //LED 关
				LED(0);
				break;
			case 6: // pwm 长鸣
				pwm(3);
				break;
			case 7: // 摄像头
				v4_control();
				break;
			default:
				system( "killall -9 madplay\n" );
				system( "killall -STOP mplayer \n" );
				system( "killall mplayer\n" );
				Stop = 0;
				show_bmp(0,0,s);
		}

		printf("recv id: %d \n", id_num);
		// udp发送数据给接收端, 接收端收到数据id后，再决定执行什么功能
		send_id(sockfd_udp, id_num);
	}

	lcd_close();
	return 0;
}
