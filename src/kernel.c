#include "define.h"

void write_bios_font_address();

int kernel_main() {

    write_bios_font_address();

    draw_char(0, 0, 0x010F, 'A');
    draw_char(1, 0, 0x010F, 'B');
    draw_char(2, 0, 0x010F, 'C');
    int aaa = hemo(12345678, 5678, 78, 76543210, 7654, 76);
    aaa = asm_sample();
    fuga(aaa);
    
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
