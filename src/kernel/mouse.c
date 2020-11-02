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
    int dx;
    int dy;
    int button;
    unsigned char buf[3];
    unsigned char state;
} MOUSE_DATA;
MOUSE_DATA mouse_data;

void init_mouse()
{
    // FIFOバッファの初期化
    fifo32_init(&mouse_input_data, MOUSE_INPUT_BUF_SIZE, mouse_input_buff);

    mouse_data.x = param_screen_x / 2;
    mouse_data.y = param_screen_y / 2;
    mouse_data.dx = 0;
    mouse_data.dy = 0;
    mouse_data.button = 0;
    mouse_data.state = MOUSE_STATE_INIT;
}

void show_mouse_state()
{
    char mouse_code[20];
    hsprintf(mouse_code, "[lcr %d %d]", mouse_data.dx, mouse_data.dy);
    if ((mouse_data.button & 0x01) != 0) {
        mouse_code[1] = 'L';
    }
    if ((mouse_data.button & 0x02) != 0) {
        mouse_code[3] = 'R';
    }
    if ((mouse_data.button & 0x04) != 0) {
        mouse_code[2] = 'C';
    }
    draw_rect(16, 460, 96, 16, COL_BLACK);
    draw_text(16, 460, mouse_code, COL_WHITE);
}

void show_mouse()
{
    // マウス移動が無い場合は無処理
    if (mouse_data.dx == 0 && mouse_data.dy == 0)
        return;

    // 以前の位置を消して、新たな位置に描画
    draw_rect(mouse_data.x, mouse_data.y, 4, 4, COL_BLACK);
    mouse_data.x += mouse_data.dx;
    mouse_data.y += mouse_data.dy;
    mouse_data.dx = 0;
    mouse_data.dy = 0;
    draw_rect(mouse_data.x, mouse_data.y, 4, 4, COL_WHITE);
}

int update_mouse()
{
    int length = mouse_input_data.len;
    for (int i = 0; i < length; i++) {
        unsigned char data = (unsigned char) fifo32_get(&mouse_input_data);
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
                mouse_data.button = mouse_data.buf[0] & 0x07;
                mouse_data.dx = mouse_data.buf[1];
                mouse_data.dy = mouse_data.buf[2];
                if ((mouse_data.buf[0] & 0x10) != 0) {
                    mouse_data.dx |= 0xffffff00;
                }
                if ((mouse_data.buf[0] & 0x20) != 0) {
                    mouse_data.dy |= 0xffffff00;
                }
                mouse_data.dy = -mouse_data.dy;
                show_mouse_state();
                show_mouse();
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
