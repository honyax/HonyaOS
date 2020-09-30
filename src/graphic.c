#include "define.h"

void draw_pixel(int x, int y, unsigned char color)
{
    if (x < 0 || x >= SCREEN_X)
        return;
    if (y < 0 || y >= SCREEN_Y)
        return;
    unsigned int addr = VRAM + SCREEN_X * y + x;
    write_mem8(addr, color);
}

void draw_line(int x1, int y1, int x2, int y2, unsigned char color)
{
    int deltaX = x1 > x2 ? x1 - x2 : x2 - x1;
    int deltaY = y1 > y2 ? y1 - y2 : y2 - y1;
    int delta = deltaX > deltaY ? deltaX : deltaY;
    int diffX = x2 - x1;
    int diffY = y2 - y1;
    for (int i = 0; i <= delta; i++) {
        draw_pixel(x1 + diffX * i / delta, y1 + diffY * i / delta, color);
    }
}

void draw_rect(int x, int y, int w, int h, unsigned char color)
{
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            draw_pixel(i, j, color);
        }
    }
}

void draw_char(int x, int y, char c, unsigned char color)
{
    unsigned char *param_font_addr = (unsigned char *)(*(unsigned int *)PARAM_FONT_ADR);
    int index = c * 16;
    for (int i = 0; i < 16; i++) {
        unsigned char data = param_font_addr[index + i];
        if ((data & 0x80) != 0) { draw_pixel(x + 0, y + i, color); }
        if ((data & 0x40) != 0) { draw_pixel(x + 1, y + i, color); }
        if ((data & 0x20) != 0) { draw_pixel(x + 2, y + i, color); }
        if ((data & 0x10) != 0) { draw_pixel(x + 3, y + i, color); }
        if ((data & 0x08) != 0) { draw_pixel(x + 4, y + i, color); }
        if ((data & 0x04) != 0) { draw_pixel(x + 5, y + i, color); }
        if ((data & 0x02) != 0) { draw_pixel(x + 6, y + i, color); }
        if ((data & 0x01) != 0) { draw_pixel(x + 7, y + i, color); }
    }
}

void write_mem8(unsigned int addr, unsigned char data) {
    unsigned char *p_addr = (unsigned char *) addr;
    *p_addr = data;
}
