#include "define.h"

extern uint _text_start;
extern uint _text_end;
extern uint _rodata_start;
extern uint _rodata_end;
extern uint _data_start;
extern uint _data_end;
extern uint _bss_start;
extern uint _bss_end;

void init_parameters();
void init_sections();
void draw_sections();

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
    for (int y = 0; y < param_screen_y; y++) {
        for (int x = 0; x < param_screen_x; x++) {
            draw_pixel(x, y, (byte)((x + y) & 0xFF));
        }
    }
#endif

    draw_line(100, 200, 500, 800, COL_GREEN);
    draw_sections();

    draw_char(16, 80, 'X', COL_WHITE);
    draw_char(24, 80, 'Y', COL_GREY);
    draw_char(32, 80, 'Z', COL_CYAN);

    draw_text(16, 120, "HonyaOS is my own operating system.", COL_WHITE);
    char test_txt[64];
    hsprintf(test_txt, "This is %d, 0x%x, 0x%X, string:%s.", 100, 0x12AB, 0x34CD, "string parameter");
    draw_text(16, 160, test_txt, COL_YELLOW);

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

void draw_sections()
{
    char addr_txt[128];
    uint _TEXT_START    = ( uint )&_text_start;
    uint _TEXT_END      = ( uint )&_text_end;
    uint _RODATA_START  = ( uint )&_rodata_start;
    uint _RODATA_END    = ( uint )&_rodata_end;
    uint _DATA_START    = ( uint )&_data_start;
    uint _DATA_END      = ( uint )&_data_end;
    uint _BSS_START     = ( uint )&_bss_start;
    uint _BSS_END       = ( uint )&_bss_end;
    hsprintf(addr_txt, "text:%X - %X rodata:%X - %X data:%X - %X bss:%X - %X",
            _TEXT_START, _TEXT_END, _RODATA_START, _RODATA_END, _DATA_START, _DATA_END, _BSS_START, _BSS_END);
    draw_text(16, 480, addr_txt, COL_CYAN);
}
