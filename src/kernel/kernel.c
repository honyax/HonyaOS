#include "define.h"

extern unsigned int _text_start;
extern unsigned int _text_end;
extern unsigned int _rodata_start;
extern unsigned int _rodata_end;
extern unsigned int _data_start;
extern unsigned int _data_end;
extern unsigned int _bss_start;
extern unsigned int _bss_end;

void write_bios_font_address();
void init_sections();

int kernel_main() {

    write_bios_font_address();
    init_sections();

    int aaa = _asm_sample();

#if 0
    for (int y = 0; y < SCREEN_Y; y++) {
        for (int x = 0; x < SCREEN_X; x++) {
            draw_pixel(x, y, (unsigned char)((x + y) & 0xFF));
        }
    }

    draw_rect(80, 80, 150, 150, 0x3);
    draw_rect(160, 160, 150, 150, 0x7);
    draw_rect(240, 240, 150, 150, 0xC);
    draw_line(100, 200, 500, 800, 0x2);
#endif

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            unsigned char color = y * 16 + x;
            draw_rect(80 * x, 64 * y, 80, 64, color);
            char str[5];
            sprintf(str, "0x%X%X", y, x);
            unsigned char text_color = ((x < 8 && y < 3) || color > 0x67) ? 0x0F : 0;
            draw_text(80 * x + 24, 64 * y + 24, str, text_color);
        }
    }

    draw_char(16, 80, 'X', 0x3);
    draw_char(24, 80, 'Y', 0x4);
    draw_char(32, 80, 'Z', 0x5);

    draw_text(16, 120, "HonyaOS is my own operating system.", 0x4);
    char test_txt[64];
    sprintf(test_txt, "This is %d, 0x%x, 0x%X, string:%s.", 100, 0x12AB, 0x34CD, "string parameter");
    draw_text(16, 160, test_txt, 0x5);

    for (;;) ;

    return 0;
}

void write_bios_font_address()
{
    // BIOSのフォントアドレスを、パラメータ領域に書き込み
    unsigned short *font_segment = (unsigned short *)(BOOT_LOAD + SECT_SIZE);
    unsigned short *font_offset  = (unsigned short *)(BOOT_LOAD + SECT_SIZE + 2);
    unsigned int font_addr = *font_segment << 4;
    font_addr += *font_offset;
    unsigned int *param_font_addr = (unsigned int *)PARAM_FONT_ADR;
    *param_font_addr = font_addr;
}

void init_sections()
{
    char addr_txt[128];
    unsigned int _TEXT_START    = ( unsigned int )&_text_start;
    unsigned int _TEXT_END      = ( unsigned int )&_text_end;
    unsigned int _RODATA_START  = ( unsigned int )&_rodata_start;
    unsigned int _RODATA_END    = ( unsigned int )&_rodata_end;
    unsigned int _DATA_START    = ( unsigned int )&_data_start;
    unsigned int _DATA_END      = ( unsigned int )&_data_end;
    unsigned int _BSS_START     = ( unsigned int )&_bss_start;
    unsigned int _BSS_END       = ( unsigned int )&_bss_end;
    sprintf(addr_txt, "text:%X - %X rodata:%X - %X data:%X - %X bss:%X - %X",
            _TEXT_START, _TEXT_END, _RODATA_START, _RODATA_END, _DATA_START, _DATA_END, _BSS_START, _BSS_END);
    draw_text(16, 480, addr_txt, 0x3);

    // BSSを初期化
    hmemset((void *)_BSS_START, 0x00, _BSS_END - _BSS_START);
}
