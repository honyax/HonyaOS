#include "define.h"

// 入力したキーコードを画面上に表示する場合は以下を有効にする
//#define KEYCODE_DBG
#define KEY_INPUT_BUF_SIZE  32

static FIFO32 key_input_data;
static int key_input_buff[KEY_INPUT_BUF_SIZE];
static int key_shift;

static char keytable0[0x80] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08/*BackSpace*/,   0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', '\n'/*Enter*/,   0,   'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0,   0,   ']', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   '\\', 0,  0,   0,   0,   0,   0,   0,   0,   0,   '\\', 0,  0
};

static char keytable1[0x80] = {
    0,   0,   '!', '\"', '#', '$', '%', '&', '\'', '(', ')', 0, '=', '~', 0x08/*BackSpace*/,   0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', '\n'/*Enter*/,   0,   'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0,   0,   '}', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   '_', 0,  0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,  0
};

void init_keyboard()
{
    // FIFOバッファの初期化
    fifo32_init(&key_input_data, KEY_INPUT_BUF_SIZE, key_input_buff);

    key_shift = 0;
}

#ifdef KEYCODE_DBG
int pos = 0;
char int_text[2];
#endif

void inthandler21(int *esp)
{
    _out8(PIC0_OCW2, 0x61);
    int data = _in8(PORT_KEYDAT);
    fifo32_put(&key_input_data, data);

#ifdef KEYCODE_DBG
    hsprintf(int_text, "%X", data);
    bg_draw_rect(16 + pos * 24, 440, 24 * 2, 16, COL_DARKBLUE);
    bg_draw_text(16 + pos * 24, 440, int_text, COL_WHITE);
    pos++;
    if (pos > 32) {
        pos = 0;
    }
#endif
}

bool try_get_key_input(char *c)
{
    if (key_input_data.len > 0) {
        int data = fifo32_get(&key_input_data);
        switch (data) {
            case 0x2a:  // 左シフトON
            case 0x36:  // 右シフトON
                key_shift = 1;
                break;
            case 0xaa:  // 左シフトOFF
            case 0xb6:  // 右シフトOFF
                key_shift = 0;
                break;
        }
        if (data < 0x80) {
            *c = key_shift == 0 ? keytable0[data] : keytable1[data];
            return TRUE;
        }
    }

    return FALSE;
}
