//
// Created by 繁星早钥 on 2024/9/11.
//

#include "v4use.h"
#include "v4l2.h"
#include <signal.h>
#include <stdio.h>
#include "lcd.h"
#include <thread>
#include "touch.h"
#include "music.h"
#include "video.h"
#include "bmp.h"


// #include "myjpeg.h"
using namespace std;

//信号处理函数：当ctrl+c时，退出
void handle_signal(int signo)
{
    if (signo == SIGINT)
    {
        Stop = 0;
    }
}

//convert实现图片格式转换
void get_stop(){
     while (Stop) {
         point g = Turn_real(get_touch()) ;
         if ( g.x > 700 ) {
             Stop = 0;
         }
     }
}

void v4_control()
{
    //初始化
    //信号处理：当ctrl+c时,产生了SIGINT中断
    //接受到该类型的信号执行函数
    Stop=1;
    signal(SIGINT, handle_signal);
    std::thread touch(get_stop);
    touch.detach();
    v4l2_init();
    //lcd_init();
    //读取一帧-格式转换-显示到本地LCD-入队
    while (Stop) {
        //point T = Turn_real(get_touch());
        struct Frame frame = v4l2_get_frame();
        char rgb888[frame.width * frame.height * 3];
        // yuv直接转rgb
        YUV422toRGB888(frame.width, frame.height, (unsigned char *)frame.addr, (unsigned char *)rgb888);
        printf("fw: %d, fh: %d\n", frame.width, frame.height);
        // -------测试yuv转jpg, jpg转rgb--------
        // unsigned char *jpgbuffer = NULL;
        // unsigned long jpgsize = 0;
        // YUYV_JPEG_MEMORY(frame.addr, frame.width, frame.height, &jpgbuffer, &jpgsize);
        // // printf("jpgbuffer: %p, jpgsize: %d\n", jpgbuffer, jpgsize);
        // JPEG_RGB_MEMORY(jpgbuffer, jpgsize, rgb888);
        // // -------------------------------------------

        // 显示rgb图像到lcd屏幕
        lcd_draw_rgb888(rgb888, frame.width, frame.height, 0, 0);
        //图片显示就在那个rgb888显示该图片.我们就用bmp显示函数显示图片就好了
        // 把帧缓冲区加入采集队列
        v4l2_enqueue(frame.index);
        // printf("%d %d \n",T.)
    }
    Stop=0;
    //释放资源
    v4l2_uninit();
    show_bmp(0,0,"Bmp/main.bmp");
    //lcd_close();
}
/*
int cam_touch(void)
{
    //打开lcd屏幕
    lcd_open();
    lcd_clear();

    //创建一条线程实时监控
    pthread_t pid;
    int touch_flag=0;
    int album_falg=0;
    int cam_flag=0;

    cam_flag=1;
    printf("打开摄像头\n");
    pthread_create(&pid,NULL,real_time_video,NULL);
    video_show_flag = 1;

    while(1)
    {
        show_location_bmp("/cam1.bmp",700,0,100,480,FB);
        touch_flag = iic();
        if(touch_flag == 5)
        {
            take_photo_flag = 1;
            printf("抓拍\n");
        }
        else if((touch_flag == 3 ||touch_flag==4) && video_show_flag ==0)
        {
            cam_flag=1;
            printf("打开摄像头\n");
            pthread_create(&pid,NULL,real_time_video,NULL);
            video_show_flag = 1;
        }
        else if(touch_flag == 9)
        {
            printf("关闭摄像头\n");
            video_show_flag = 0;
            usleep(100000);
            break;
        }
        else if(touch_flag == 7)
        {
            video_show_flag = 0;
            printf("相册\n");
            album_falg=album_cam(1);
            if(album_falg==2) break;
            if(album_falg==1)
            {
                printf("打开摄像头\n");
                pthread_create(&pid,NULL,real_time_video,NULL);
                video_show_flag = 1;
                cam_flag=1;
            }
        }

    }


    // //关闭lcd屏幕
    // lcd_close();
    return 0;
}
*/