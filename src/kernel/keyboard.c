#include "define.h"

#define KEY_INPUT_BUF_SIZE  32

static FIFO32 key_input_data;
static int key_input_buff[KEY_INPUT_BUF_SIZE];
static int key_input_pos;

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

    key_input_pos = 0;
}

void inthandler21(int *esp)
{
    _out8(PIC0_OCW2, 0x61);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&key_input_data, data);
}

bool try_get_key_input(char *c)
{
    if (key_input_data.len > 0) {
        int data = fifo32_get(&key_input_data);
        if (data < 0x80) {
            *c = keytable[data];
            return TRUE;
        }
    }

    return FALSE;
}
