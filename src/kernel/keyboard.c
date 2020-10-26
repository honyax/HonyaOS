#include "define.h"

#define KEY_INPUT_BUF_SIZE  32

FIFO32 key_input_data;
int key_input_buff[KEY_INPUT_BUF_SIZE];

void init_keyboard()
{
    // FIFOバッファの初期化
    fifo32_init(&key_input_data, KEY_INPUT_BUF_SIZE, key_input_buff);
}

void inthandler21(int *esp)
{
    _out8(PIC0_OCW2, 0x61);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&key_input_data, data);
}
