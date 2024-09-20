#ifndef _LCD_SHOW_H
#define _LCD_SHOW_H

void show_a_color(int color);
void display_point(int x, int y, int color);
void lcd_close();
int lcd_init();
void display_Tangle(int x0,int y0,int h,int color);
void display_Rectangle(int x0,int y0,int w,int h,int color);
void display_Circle(int x0, int y0, int r, int color);

#endif

















