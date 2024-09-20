#ifndef _BMP_H
#define _BMP_H

int show_bmp(int x0, int y0, const char *filename );
int zoom_bmp(int x0,int y0,double x_base,double y_base,char *filename);
void YUV422toRGB888(int width, int height, unsigned char *src, unsigned char *dst);
void lcd_draw_rgb888(char *rgb888, int width, int height, int x, int y);
#endif 
