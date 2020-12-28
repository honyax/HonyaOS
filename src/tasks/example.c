#include "../syscall/define.h"
#include "../lib/define.h"

void main()
{
    _sc_example(0x100, 0x200, 0x500, 0x1000, 0x10000);
    int hhmmss = _sc_get_current_time();
    char s[16];
    hsprintf(s, "%X:%X:%X", (hhmmss & 0xFF0000) >> 16, (hhmmss & 0xFF00) >> 8, hhmmss & 0xFF);
    _sc_bg_draw_text(80, 700, s, COL_WHITE);

    for (;;) {
    }
}
