#include "../syscall/define.h"

void main()
{
    _sc_example(0x100, 0x200, 0x500, 0x1000, 0x10000);
    _sc_bg_draw_text(80, 700, (int) ("hogehoge"), 10);

    for (;;) {
    }
}
