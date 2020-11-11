#include "define.h"

#define KEY_INPUT_BUF_SIZE  32

FIFO32 key_input_data;
int key_input_buff[KEY_INPUT_BUF_SIZE];
int key_input_pos = 0;

static char keytable[0x80] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08,   0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0x0a,   0,   'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
};

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
        if (data < 0x80) {
            char key_code[4];
            int pos_x = 16 + 8 * key_input_pos;
            key_input_pos++;
            // 画面の右はじの方に行ったら位置を初期化
            if (pos_x > param_screen_x - 40) {
                key_input_pos = 0;
            }
            hsprintf(key_code, "%c", keytable[data]);
            draw_rect(pos_x, 440, 8, 16, COL_BLACK);
            draw_text(pos_x, 440, key_code, COL_WHITE);

            // TODO: sキー押下でタスク切替するテスト
            if (keytable[data] == 'S') {
                task_switch();
            }
        }
    }
    return length;
}

void inthandler21(int *esp)
{
    _out8(PIC0_OCW2, 0x61);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&key_input_data, data);
}
