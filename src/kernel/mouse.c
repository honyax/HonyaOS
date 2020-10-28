#include "define.h"

#define MOUSE_INPUT_BUF_SIZE    128     // マウスのインプットデータを格納するバッファのサイズ

FIFO32 mouse_input_data;
int mouse_input_buff[MOUSE_INPUT_BUF_SIZE];

#define MOUSE_STATE_INIT        0   // 最初の0xFAが来る前の状態
#define MOUSE_STATE_PHASE_1     1   // １バイト目のデータを待っている状態
#define MOUSE_STATE_PHASE_2     2   // ２バイト目のデータを待っている状態
#define MOUSE_STATE_PHASE_3     3   // ３バイト目のデータを待っている状態

typedef struct
{
    int x;
    int y;
    int button;
    unsigned char buf[3];
    unsigned char state;
} MOUSE_DATA;
MOUSE_DATA mouse_data;

void init_mouse()
{
    // FIFOバッファの初期化
    fifo32_init(&mouse_input_data, MOUSE_INPUT_BUF_SIZE, mouse_input_buff);

    mouse_data.state = MOUSE_STATE_INIT;
}

int update_mouse()
{
    int length = mouse_input_data.len;
    for (int i = 0; i < length; i++) {
        unsigned char data = (unsigned char) fifo32_get(&mouse_input_data);
        int pos_x = 0;
        switch (mouse_data.state) {
            case MOUSE_STATE_INIT:
                if (data == 0xfa) {
                    mouse_data.state = MOUSE_STATE_PHASE_1;
                }
                continue;
            case MOUSE_STATE_PHASE_1:
                // 正しいマウスデータの１バイト目か念の為チェック
                if ((data & 0xC8) == 0x08) {
                    mouse_data.buf[0] = data;
                    mouse_data.state = MOUSE_STATE_PHASE_2;
                }
                break;
            case MOUSE_STATE_PHASE_2:
                mouse_data.buf[1] = data;
                mouse_data.state = MOUSE_STATE_PHASE_3;
                break;
            case MOUSE_STATE_PHASE_3:
                mouse_data.buf[2] = data;
                mouse_data.state = MOUSE_STATE_PHASE_1;
                char mouse_code[16];
                sprintf(mouse_code, "%X %X %X", mouse_data.buf[0], mouse_data.buf[1], mouse_data.buf[2]);
                draw_rect(16, 460, 64, 16, COL_BLACK);
                draw_text(16, 460, mouse_code, COL_WHITE);
                break;
        }
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
