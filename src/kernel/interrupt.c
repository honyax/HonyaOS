#include "define.h"

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
    _out8(PIC0_IMR, 0xFF);  // 全ての割り込みを受け付けない
    _out8(PIC1_IMR, 0xFF);  // 全ての割り込みを受け付けない

    _out8(PIC0_ICW1, 0x11); // エッジトリガモード
    _out8(PIC0_ICW2, 0x20); // IRQ0-7は、INT20-27で受ける
    _out8(PIC0_ICW3, 1 << 2);   // PIC1はIRQ2にて接続
    _out8(PIC0_ICW4, 0x01); // ノンバッファモード

    _out8(PIC1_ICW1, 0x11); // エッジトリガモード
    _out8(PIC1_ICW2, 0x28); // IRQ8-15は、INT28-2Fで受ける
    _out8(PIC1_ICW3, 2);    // PIC1はIRQ2にて接続
    _out8(PIC1_ICW4, 0x01); // ノンバッファモード

    _out8(PIC0_IMR, 0xFB);  // 11111011 PIC1以外は全て禁止
    _out8(PIC1_IMR, 0xFF);  // 11111111 全ての割り込みを受け付けない
}
