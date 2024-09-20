//
// Created by 刘云志· on 2024/9/7.
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
const string command = "madplay";
const int Music_Size = 4;
char Music_file[10][30] = {
    //"Video/Jar.mp3","Video/1.mp3","Video/GEM.mp3","mayun.mp3"
   "Video/1.mp3","Video/Jar.mp3","Video/mayun.mp3","Video/GEM.mp3"
};
char Main_[5][30] = {
    "Bmp/main.bmp","Bmp/Music.bmp"
};
int control_music(int *stop) {
    Stop = 1;
    int i = 0;
    int flag=0;
    show_bmp(0,0,Main_[1]);   //图片显示
    system( "killall -9 madplay\n" );
    system( "killall -STOP mplayer \n" );
    system( "killall mplayer\n" );
    system((command + " " + Music_file[i] + " &\n").c_str() );  //mplayer xxx.mp4
    while (true) {
        point T = Turn_real(get_touch());
        if (T.x < 110 && T.x > 30&& T.y < 480 && T.y > 390) { // 音量减

        }
        else if (T.x < 245 && T.x > 165 && T.y < 480 && T.y > 390) { // 上一首
            //system("mplayer < xxx.mp4");
            i = (i+Music_Size-1) % Music_Size;
            system( "killall -9 madplay\n" );
            //system((command + " q " + Music_file[i]+ "\n").c_str());
           // system( "killall mplayer\n" );
            system((command + " "+ Music_file[i]+ " &\n").c_str() );
        } else if (T.x < 420 && T.x > 325 && T.y < 480 && T.y > 390) {// 暂停
           // system("mplayer p xxx.mp4");
           // i = (i+mod+1) % mod;
            flag++;
            //system((command + " p " + Music_file[i]+ "\n").c_str());
              // p or SPACE       pause movie (press any key to continue)
            if(flag%2==1){system("killall -STOP madplay &");}
            else{system("killall -CONT madplay &");}
        } else if (T.x < 590 && T.x > 500 && T.y < 480 && T.y > 390) { // 下一首
             //< or >       step backward/forward in playlist
              //system("mplayer > xxx.mp4");
             i = (i+Music_Size+1) % Music_Size;
            // system((command + " q " + Music_file[i]+ "\n").c_str());
            //system( "killall mplayer\n" );
            system( "killall -9 madplay\n" );
            system((command + " "+ Music_file[i]+ " &\n").c_str() );
           // system((command + " > " + Music_file[i]+ "\n").c_str());
        } else if (T.x < 790 && T.x >=705 && T.y < 480 && T.y > 390) { // 音量加

        } else if (T.x <= 800 && T.x >= 700 && T.y <90 && T.y >0) { // 退出
            // system("mplayer  q xxx.mp4");
            //system((command + " q " + Music_file[i]+ "\n").c_str());
            //system( "killall mplayer\n" );
            // system( "killall -9 madplay\n" );
            Stop = 0;
            show_bmp(0,0,Main_[0]);
            break;
        }

    }
    return 1;
}
//system("mplayer -zoom -x 800 -y 480 xxx.mp4");