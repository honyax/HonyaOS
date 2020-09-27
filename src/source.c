#include "header.h"

void fuga(int param) {
    if (param < 1)
        return;
    draw_char(0, 1, 0x0402, '0');
    draw_char(1, 1, 0x0212, '1');
    draw_char(2, 1, 0x0212, '_');
}

int hemo(int a, int b, int c, int d, int e, int f) {
    if (a < 10) return 0x11111111;
    if (b < 11) return 0x22222222;
    if (c < 12) return 0x33333333;
    if (d < 20) return 0x44444444;
    if (e < 21) return 0x55555555;
    if (f < 22) return 0x66666666;
    return 0x77777777;
}
