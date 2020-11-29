#include "define.h"

void init_parameters();
void init_sections();

// 以下は他のファイルからも参照可能なグローバル変数
byte* param_font_adr;
ushort param_screen_x;
ushort param_screen_y;
uint param_vram;

int kernel_main() {

    init_parameters();
    init_sections();
    init_descriptor();
    init_pic();
    init_pit();
    init_keyboard();
    init_mouse();
    enable_mouse_keyboard();
    init_memory();
    init_timer();
    init_paging();
    init_task();
    init_window();

    _sti();

#if 0
    draw_color_test();
#endif

    for (;;) {
        update_interrupt();
        update_timer();
        update_window();
    }

    return 0;
}

// ブートローダから渡されたパラメータを設定
void init_parameters()
{
    // フォントアドレス
    param_font_adr = (byte *)(*(uint *)PARAM_FONT_ADR);
    param_screen_x = (ushort)(*(ushort *)PARAM_SCREEN_X);
    param_screen_y = (ushort)(*(ushort *)PARAM_SCREEN_Y);
    param_vram = (uint)(*(uint *)PARAM_VRAM);
}

void init_sections()
{
    uint _BSS_START     = ( uint )&_bss_start;
    uint _BSS_END       = ( uint )&_bss_end;

    // BSSを初期化
    hmemset((void *)_BSS_START, 0x00, _BSS_END - _BSS_START);
}
