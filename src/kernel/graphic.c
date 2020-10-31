#include "define.h"

void draw_pixel(int x, int y, unsigned char color)
{
    if (x < 0 || x >= param_screen_x)
        return;
    if (y < 0 || y >= param_screen_y)
        return;
    unsigned int addr = param_vram + param_screen_x * y + x;
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
    int index = c * 16;
    for (int i = 0; i < 16; i++) {
        unsigned char data = param_font_adr[index + i];
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

void draw_text(int x, int y, unsigned char* text, unsigned char color)
{
    for (int i = 0; *(text + i) != 0x00; i++) {
        draw_char(x + 8 * i, y, text[i], color);
    }
}

void draw_color_test()
{
    int dx = param_screen_x / 16;
    int dy = param_screen_y / 16;

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            unsigned char color = y * 16 + x;
            draw_rect(dx * x, dy * y, dx, dy, color);
            char str[5];
            sprintf(str, "0x%X%X", y, x);
            unsigned char text_color;
            if ((x < 10 && y < 3) ||
                (color >= 0x36 && color <= 0x39) ||
                color > 0x67) {
                text_color = COL_FCFCFC;
            } else {
                text_color = COL_000000;
            }
            draw_text(dx * x + (dx / 2 - 16), dy * y + (dy / 2 - 8), str, text_color);
        }
    }
}
