#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <sys/mman.h>
#include <cmath>
#include <cstdlib>
#include "lcd.h"
#include "bmp.h"
using namespace std;
void YUV422toRGB888(int width, int height, unsigned char *src, unsigned char *dst)
{
    int line, column;
    unsigned char *py, *pu, *pv;
    unsigned char *tmp = dst;

    py = src;
    pu = src + 1;
    pv = src + 3;
#define CLIP(x) ((x) >= 0xFF ? 0xFF : ((x) <= 0x00 ? 0x00 : (x)))
    for (line = 0; line < height; ++line)
    {
        for (column = 0; column < width; ++column)
        {
            *tmp++ = CLIP((int)*py + ((int)*pu - 128) + ((103 * ((int)*pu - 128)) >> 8));
            *tmp++ = CLIP((int)*py - (88 * ((int)*pv - 128) >> 8) - ((183 * ((int)*pu - 128)) >> 8));
            *tmp++ = CLIP((int)*py + ((int)*pv - 128) + ((198 * ((int)*pv - 128)) >> 8));
            // increase py every time
            py += 2;
            // increase pu,pv every second time
            if ((column & 1) == 1)
            {
                pu += 4;
                pv += 4;
            }
        } //end for col
    }      //end for line
}
void lcd_draw_rgb888(char *rgb888, int width, int height, int x, int y)
{
    int i, j;
    char r, g, b;
    int color;
    char *p = rgb888;
    for (i = 0; i < height; ++i)
    {
        for (j = 0; j < width; ++j)
        {
            b = *p++;
            g = *p++;
            r = *p++;
            color = r << 16 | g << 8 | b;
            display_point(j + x,y + i,color);
        }
    }
}
int show_bmp(int x0, int y0, const char *filename ) {
    //1. 打开图片
    int fd = open(filename, O_RDONLY );
    if (fd == -1 ) {
        perror("Fail to open BMP");
        return -1;
    }
    //2.读取数据       宽，高，色深
    int width = 0, depth = 0, height = 0;
    int laizi = 0;  // 填充的无用字节数
    int line_size = 0;
    lseek ( fd , 0x12 , SEEK_SET);
    read ( fd, &width , 4); // 记录宽
    lseek ( fd , 0x16 , SEEK_SET);
    read ( fd, &height , 4); // 记录高
    lseek ( fd , 0x1c , SEEK_SET);
    read ( fd, &depth , 2);   // 记录 色深
    laizi = (4 - (abs(width) * (depth / 8) ) ) % 4;
    line_size = abs(width) * (depth / 8) + laizi;  // 每行的字节数
    unsigned char buf[abs(height * line_size) ];
    lseek ( fd , 0x36, SEEK_SET ) ;
    read( fd , buf , abs(height) * line_size );
    //3.显示
    int a = 0;
    int num = 0;
    for ( int j = 0; j < abs(height) ; j++ ) {
        for ( int i = 0; i < abs(width) ; i++ ) {
            int b = buf[num++];
            int g = buf[num++];
            int r = buf[num++];
            if (depth == 32) {
                a = buf[num++];
            }
            int color = (a << 24) | (r << 16) | (g << 8) | b;
            // 画点
            display_point( width >= 0 ? i + x0 : x0 + abs(width)  - 1 - i ,
                           height < 0 ? j + y0 : y0 + abs(height) - 1 - j, color);
        }
        num += laizi;  // 跳过无效的数据
    }
    //4.关闭图片
    if (close(fd) == -1) {
        printf("Fail to close BMP %s.\n", filename);
        return -1;
    }
    return 1;
}
int zoom_bmp(int x0, int y0, double x_base, double y_base, char *filename) {
    x0 = y0 = 0;
    //assert(x_base * y_base> 5e-3 && x_base * y_base < 200);
    //1. 打开图片
    int fd = open(filename, O_RDONLY );
    if (fd == -1 ) {
        perror("Fail to open BMP");
        return -1;
    }
    //2.读取数据       宽，高，色深
    int width = 0, depth = 0, height = 0;
    int laizi = 0;  // 填充的无用字节数
    int line_size = 0;
    lseek ( fd , 0x12 , SEEK_SET);
    read ( fd, &width , 4); // 记录宽
    lseek ( fd , 0x16 , SEEK_SET);
    read ( fd, &height , 4); // 记录高
    lseek ( fd , 0x1c , SEEK_SET);
    read ( fd, &depth , 2);   // 记录 色深
    laizi = (4 - (abs(width) * (depth / 8) ) ) % 4;
    line_size = abs(width) * (depth / 8) + laizi;  // 每行的字节数
    unsigned char buf[abs(height * line_size) ];
    lseek ( fd , 0x36, SEEK_SET ) ;
    read( fd , buf , abs(height) * line_size );
    //3.显示
    int b, g, r, a = 0;
    int i, j, num = 0;
     // sleep(1);
    // 坐标映射
    // x0 = 400 - w/2 y0 = 240 - h/2 图片置于中心
    x0 = 400 - abs(width)* x_base/2 ,y0 = 240 - abs(height) * y_base/2;

    int pre_color[abs(width) ][abs(height)];  // 记录原本图片的像素点 ]
    for ( j = 0; j < abs(height) ; j++ ) {
        for ( i = 0; i < abs(width) ; i++ ) {
            b = buf[num++];
            g = buf[num++];
            r = buf[num++];
            if (depth == 32) {
                a = buf[num++];
            }
            int color = (a << 24) | (r << 16) | (g << 8) | b;
            // 计算缩放后的偏移
            // 画点 此处变为记录 
            /*display_point( width >= 0 ? i + x0 : x0 + abs(width)  - 1 - i ,
                           height < 0 ? j + y0 : y0 + abs(height) - 1 - j, color); */
            int nx = width >= 0 ? i  : + abs(width)  - 1 - i,ny = height < 0 ? j :  + abs(height) - 1 - j;
            if (nx <abs(width) && nx >=0 && ny < abs(height) && ny >= 0) {
                pre_color[nx][ny] = color;
            }
        }
        num += laizi;  // 跳过无效的数据
    }
    show_a_color(0x000000);
    for ( i = max(0,-x0); i < abs(width) * x_base && i + x0 < 800; i++ ) {
        for ( j = max(0,-y0); j < abs(height) * y_base && j + y0 < 480 ; j++ ) {
            int ni = (i /x_base + 0.5) ,nj = (j /y_base+ 0.5 ) ;
            if (ni <abs(width) && ni >=0 && nj < abs(height) && nj >= 0) {
                display_point(i+x0 ,j+y0 ,pre_color[ni][nj]);
            } else {
                break;
            }

        }
        num += laizi;  // 跳过无效的数据
    }
    //4.关闭图片
    if (close(fd) == -1) {
        printf("Fail to close BMP %s.\n", filename);
        return -1;
    }
    return 1;
}


