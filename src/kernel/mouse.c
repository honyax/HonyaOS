#include "define.h"

#define MOUSE_INPUT_BUF_SIZE    50

FIFO32 mouse_input_data;
int mouse_input_buff[MOUSE_INPUT_BUF_SIZE];

void init_mouse()
{
    // FIFOバッファの初期化
    fifo32_init(&mouse_input_data, MOUSE_INPUT_BUF_SIZE, mouse_input_buff);
}

void inthandler2c(int *esp)
{
    _out8(PIC1_OCW2, 0x64);
    _out8(PIC0_OCW2, 0x62);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&mouse_input_data, data);
}
