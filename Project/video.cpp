//
// Created by 徐国翔· on 2024/9/7.
//
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <sys/mman.h>
#include <cstdlib>
#include "bmp.h"
#include "music.h"
#include <string>
#include "touch.h"
#include <linux/input.h>
using namespace std;
const string command = "mplayer -zoom -x 700 -y 385 -quiet";
const int Video_Size = 4;
string page = "Bmp/Music.bmp";
char Video_file[10][30] = {
   "Video/manbo.mp4","Video/hf.mp4","Video/pvz.mp4","Video/kenan.mp4"
};

int control_video (int *stop) {
    Stop = 1;
    int i = 0;
    int flag = 0 ;
    //int voice_size[100] = "50" ;
    system( "killall -STOP mplayer \n" );
    system( "killall mplayer\n" );
    system( "killall -9 madplay\n" );
    system( "select video output driver\n" );

    system((command + " "+ Video_file[i]+ " & \n").c_str() );  //mplayer xxx.mp4
    show_bmp(0,0,page.c_str());
    while ( true ) {
        point T = Turn_real(get_touch());
        if (T.x < 110 && T.x > 30&& T.y < 480 && T.y > 390) { // 静音
          //system( ( string("mute ") + "1" +" \n").c_str() );
        }
        else if (T.x < 245 && T.x > 165 && T.y < 480 && T.y > 390) { // 上一首

            i = (i+ Video_Size-1) % Video_Size;
            system( "killall mplayer\n" );
            system((command + " "+ Video_file[i]+ " & \n").c_str() );

        } else if (T.x < 420 && T.x > 325 && T.y < 480 && T.y > 390) {// 暂停

           // system("mplayer p xxx.mp4");
           // i = (i+mod+1) % mod;
            flag ++;
            //system((command + " p " + Music_file[i]+ "\n").c_str());
              // p or SPACE       pause movie (press any key to continue)
            if(flag%2==1) {
                system( "killall -STOP mplayer \n" );
            }
            else {
                system("killall -CONT mplayer \n");
            }
        } else if (T.x < 590 && T.x > 500 && T.y < 480 && T.y > 390) { // 下一首
            i = (i+ Video_Size+1) % Video_Size;
            system( "killall mplayer\n" );
            system((command + " "+ Video_file[i]+ " & \n").c_str() );
        } else if (T.x < 790 && T.x >=705 && T.y < 480 && T.y > 390) { // 取消静音
            //system( ( string("mute ") + "0" +" \n").c_str() );
        } else if (T.x <= 800 && T.x >= 700 && T.y <90 && T.y >0) { // 退出
            system( "killall -STOP mplayer \n" );
            system( "killall mplayer\n" );
            Stop=0;
            show_bmp(0,0,"Bmp/main.bmp");
            break;
        }

    }


    //Stop=0;
    return 1;
}
//system("mplayer -zoom -x 800 -y 480 xxx.mp4");

