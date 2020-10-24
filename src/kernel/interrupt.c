#include "define.h"

// PIC(Programmable Interrupt Controller)
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

void init_pic()
{
    _out8(PIC0_IMR,  0xFF); // 全ての割り込みを受け付けない
    _out8(PIC1_IMR,  0xFF); // 全ての割り込みを受け付けない

    _out8(PIC0_ICW1, 0x11); // エッジトリガモード
    _out8(PIC0_ICW2, 0x20); // IRQ0-7は、INT20-27で受ける
    _out8(PIC0_ICW3, 0x04); // PIC1はIRQ2にて接続
    _out8(PIC0_ICW4, 0x01); // ノンバッファモード

    _out8(PIC1_ICW1, 0x11); // エッジトリガモード
    _out8(PIC1_ICW2, 0x28); // IRQ8-15は、INT28-2Fで受ける
    _out8(PIC1_ICW3, 0x02); // PIC1はIRQ2にて接続
    _out8(PIC1_ICW4, 0x01); // ノンバッファモード

    _out8(PIC0_IMR,  0xFB); // 11111011 PIC1以外は全て禁止
    _out8(PIC1_IMR,  0xFF); // 11111111 全ての割り込みを受け付けない
}

void enable_mouse_keyboard()
{
    _out8(PIC0_IMR, 0xF9);  // 11111001 PIC1, Keyboardを許可
    _out8(PIC1_IMR, 0xEF);  // 11101111 マウスを許可
}


// PIT(Programmable Interval Timer)
// ポート アドレス
#define PIT_REG_COUNTER0    0x0040
#define PIT_REG_CONTROL     0x0043
// 入力 クロック CLK 0
#define DEF_PIT_CLOCK       1193181.67
// counter mode
#define DEF_PIT_COM_MODE_SQUAREWAVE 0x06
// data transfer
#define DEF_PIT_COM_RL_DATA         0x30
// counter
#define DEF_PIT_COM_COUNTER0        0x00

void init_pit()
{
    int freq = 100;
    unsigned short count = (unsigned short)(DEF_PIT_CLOCK / freq);
    unsigned char command = DEF_PIT_COM_MODE_SQUAREWAVE | DEF_PIT_COM_RL_DATA | DEF_PIT_COM_COUNTER0;

    _out8(PIT_REG_CONTROL, command);
    _out8(PIT_REG_COUNTER0, (unsigned char)(count & 0xFF));
    _out8(PIT_REG_COUNTER0, (unsigned char)((count >> 8) & 0xFF));
}

void inthandler21(int *esp)
{
    draw_text(16, 240, "1", COL_WHITE);
}

void inthandler_default(int *esp)
{
    return;
}
