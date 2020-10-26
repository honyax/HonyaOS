#include "define.h"

#define PORT_KEYDAT         0x60

#define KEY_INPUT_BUF_SIZE  32

FIFO32 key_input_data;
int key_input_buff[KEY_INPUT_BUF_SIZE];

void init_keyboard()
{
    fifo32_init(&key_input_data, KEY_INPUT_BUF_SIZE, key_input_buff);
}

void update_keyboard()
{
    _cli();
    if (key_input_data.len > 0) {
        for (int i = 0; i < key_input_data.len; i++) {
            char keyCode[4];
            int pos_x = 16 + 24 * key_input_data.pos_r;
            sprintf(keyCode, "%X", fifo32_get(&key_input_data));
            draw_rect(pos_x, 440, 16, 16, COL_BLACK);
            draw_text(pos_x, 440, keyCode, COL_WHITE);
        }
        _sti();
    } else {
        _stihlt();
    }
}

void inthandler21(int *esp)
{
    _out8(PIC0_OCW2, 0x61);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&key_input_data, data);
}
