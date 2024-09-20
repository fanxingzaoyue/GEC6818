//
// Created by 刘云志· on 2024/9/7.
//
#include "photo.h"
#include "bmp.h"
#include "lcd.h"
#include "music.h"
#include "touch.h"
const int Photo_Size = 7;

char Photo_file[20][30] = {
    "Bmp/pic1.bmp","Bmp/pic2.bmp","Bmp/pic3.bmp",
    "Bmp/test1.bmp","Bmp/test2.bmp","Bmp/test3.bmp","Bmp/test4.bmp",
};
void show_photo() {
    Stop = 1;
    //lcd_init();
    int index = 0;
    double base = 1;
    zoom_bmp(0,0,base,base,Photo_file[index]);
    while (true) {
        int g = get_direction();
        if (g == LEFT ) { // 下一张
            index ++;
        } else if (g == RIGHT ) {
            index --;
        } else if (g == UP ) {   // 放大
            if (base < 1.5 * 1.5 ) base *= 1.5;
        } else if (g == DOWN) {
            if (base > 1.0 / 1.5 / 1.5) base /= 1.5;
        } else if (g == QUIT) {   //退出相册
            Stop = 0;
            show_bmp(0,0,"Bmp/main.bmp");
            break;
        }
        index = (index + Photo_Size ) % Photo_Size;
        zoom_bmp(0,0,base,base,Photo_file[index]);
    }

    //lcd_close();
}
