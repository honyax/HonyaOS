#include "define.h"

void draw_pixel(int x, int y, unsigned char color)
{
    unsigned int addr = VRAM + SCREEN_X * y + x;
    write_mem8(addr, color);
}

void write_mem8(unsigned int addr, unsigned char data) {
    unsigned char *p_addr = (unsigned char *) addr;
    *p_addr = data;
}
