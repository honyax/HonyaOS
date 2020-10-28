#include "define.h"

#define KEY_INPUT_BUF_SIZE  32

FIFO32 key_input_data;
int key_input_buff[KEY_INPUT_BUF_SIZE];

void init_keyboard()
{
    // FIFOバッファの初期化
    fifo32_init(&key_input_data, KEY_INPUT_BUF_SIZE, key_input_buff);
}

int update_keyboard()
{
    int length = key_input_data.len;
    for (int i = 0; i < length; i++) {
        int data = fifo32_get(&key_input_data);
        char key_code[4];
        int pos_x = 16 + 24 * key_input_data.pos_r;
        sprintf(key_code, "%X", data);
        draw_rect(pos_x, 440, 16, 16, COL_BLACK);
        draw_text(pos_x, 440, key_code, COL_WHITE);
    }
    return length;
}

void inthandler21(int *esp)
{
    _out8(PIC0_OCW2, 0x61);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&key_input_data, data);
}
