#include "define.h"

void write_bios_font_address();

int kernel_main() {

    write_bios_font_address();

    int aaa = asm_sample();

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

    char text[] = "HonyaOS is my own operating system.";
    draw_text(16, 120, text, 0x44);

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
