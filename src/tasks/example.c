#include "../syscall/define.h"
#include "../lib/define.h"

void main()
{
    RECT rect;
    rect.x = 100;
    rect.y = 100;
    rect.w = 100;
    rect.h = 80;
    int win_handle = _sc_win_create(&rect);
    rect.x = 0;
    rect.y = 0;
    _sc_win_draw_rect(win_handle, &rect, COL_WHITE);
    rect.x = 10;
    rect.y = 10;
    rect.w = 80;
    rect.h = 60;
    _sc_win_draw_rect(win_handle, &rect, COL_DARKRED);

    rect.x = 18;
    rect.y = 8;
    rect.w = 64;
    rect.h = 64;
    byte data[64*64];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            int x = i - 32;
            int y = j - 32;
            byte color;
            if (x * x + y * y < 625) {
                color = COL_DARKGREEN;
            } else {
                color = COL_NONE;
            }
            data[64 * j + i] = color;
        }
    }
    _sc_win_draw_bytes(win_handle, &rect, data);

    rect.x = 18;
    rect.y = 32;
    rect.w = 64;
    rect.h = 16;

    int hhmmss;
    char s[16];
    for (;;) {
        hhmmss = _sc_get_current_time();
        hsprintf(s, "%X:%X:%X", (hhmmss & 0xFF0000) >> 16, (hhmmss & 0xFF00) >> 8, hhmmss & 0xFF);
        _sc_win_draw_rect(win_handle, &rect, COL_DARKRED);
        _sc_win_draw_text(win_handle, 18, 32, s, COL_WHITE);
        _sc_sleep(1000);
    }
}
