#include "define.h"

#define MOUSE_INPUT_BUF_SIZE    50

FIFO32 mouse_input_data;
int mouse_input_buff[MOUSE_INPUT_BUF_SIZE];

void init_mouse()
{
    // FIFOバッファの初期化
    fifo32_init(&mouse_input_data, MOUSE_INPUT_BUF_SIZE, mouse_input_buff);
}

int update_mouse()
{
    int length = mouse_input_data.len;
    for (int i = 0; i < length; i++) {
        int data = fifo32_get(&mouse_input_data);
        char mouse_code[4];
        int pos_x = 16 + 24 * mouse_input_data.pos_r;
        sprintf(mouse_code, "%X", data);
        draw_rect(pos_x, 460, 16, 16, COL_BLACK);
        draw_text(pos_x, 460, mouse_code, COL_WHITE);
    }
    return length;
}

void inthandler2c(int *esp)
{
    _out8(PIC1_OCW2, 0x64);
    _out8(PIC0_OCW2, 0x62);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&mouse_input_data, data);
}
