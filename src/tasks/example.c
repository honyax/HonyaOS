#include "../syscall/define.h"
#include "../lib/define.h"

void main()
{
    _sc_example(0x100, 0x200, 0x500, 0x1000, 0x10000);
    int hhmmss = _sc_get_current_time();
    char s[16];
    hsprintf(s, "%X:%X:%X", (hhmmss & 0xFF0000) >> 16, (hhmmss & 0xFF00) >> 8, hhmmss & 0xFF);
    _sc_bg_draw_text(80, 700, s, COL_WHITE);
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
    _sc_win_draw_rect(win_handle, &rect, COL_RED);

    for (;;) {
    }
}
