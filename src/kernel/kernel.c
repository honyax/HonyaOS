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
#endif

    draw_rect(80, 80, 150, 150, 0x33);
    draw_rect(160, 160, 150, 150, 0x77);
    draw_rect(240, 240, 150, 150, 0xCC);
    draw_line(100, 200, 500, 800, 0x22);

    draw_char(16, 80, 'X', 0x33);
    draw_char(24, 80, 'Y', 0x44);
    draw_char(32, 80, 'Z', 0x55);

    draw_text(16, 120, "HonyaOS is my own operating system.", 0x44);
    char test_txt[64];
    sprintf(test_txt, "This is %d, 0x%x, 0x%X, string:%s.", 100, 0x12AB, 0x34CD, "string parameter");
    draw_text(16, 160, test_txt, 0x55);

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
    draw_text(16, 480, addr_txt, 0x33);

    // BSSを初期化
    hmemset((void *)_BSS_START, 0x00, _BSS_END - _BSS_START);
}
