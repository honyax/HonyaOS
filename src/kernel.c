#include "define.h"

void write_bios_font_address();

int kernel_main() {

    write_bios_font_address();

    //draw_char(0, 0, 0x010F, 'A');
    //draw_char(1, 0, 0x010F, 'B');
    //draw_char(2, 0, 0x010F, 'C');
    int aaa = hemo(12345678, 5678, 78, 76543210, 7654, 76);
    aaa = asm_sample();
    fuga(aaa);

    unsigned int addr;
    for (int y = 0; y < SCREEN_Y; y++) {
        for (int x = 0; x < SCREEN_X; x++) {
            draw_pixel(x, y, (unsigned char)((x + y) & 0xFF));
        }
    }

    draw_rect(20, 20, 150, 150, 0x33);
    draw_rect(40, 40, 150, 150, 0x77);
    draw_rect(60, 60, 150, 150, 0xCC);
    draw_line(100, 200, 500, 800, 0x7F);
    
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

