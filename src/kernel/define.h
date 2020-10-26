#pragma once

// define.sとの共通化
#define     BOOT_SIZE               (1024 * 8)      // ブートサイズ
#define     KERNEL_SIZE             (1024 * 8)      // カーネルサイズ

#define     BOOT_LOAD               0x7C00          // ブートプログラムのロード位置
#define     BOOT_END				(BOOT_LOAD + BOOT_SIZE)

#define     PARAM_BASE              0x00101000      // パラメータ領域の開始位置
#define     KERNEL_LOAD             0x00103000      // カーネルのロード位置（プロテクトモード）

#define     SECT_SIZE               (512)           // セクタサイズ

#define     BOOT_SECT               (BOOT_SIZE   / SECT_SIZE)   // ブートプログラムのセクタ数
#define     KERNEL_SECT				(KERNEL_SIZE / SECT_SIZE)	// カーネルのセクタ数

#define     E820_RECORD_SIZE		20

#if 1
    #define VBE_MODE                (0x107)         // 1280 x 1024 x 8bitカラー
    #define SCREEN_X                (1280)          // 画面サイズ（横）
    #define SCREEN_Y                (1024)          // 画面サイズ（縦）
#else
    #define VBE_MODE                (0x105)         // 1024 x 768 x 8bitカラー
    #define SCREEN_X                (1024)          // 画面サイズ（横）
    #define SCREEN_Y                (768)           // 画面サイズ（縦）
#endif

#define     VRAM                    (0xe0000000)    // VRAMのアドレス

#define     PARAM_FONT_ADR          PARAM_BASE      // フォントアドレスの位置



// カラーコード
#define     COL_000000      0
#define     COL_0000A8      1
#define     COL_00A800      2
#define     COL_00A8A8      3
#define     COL_A80000      4
#define     COL_A800A8      5
#define     COL_A85400      6
#define     COL_A8A8A8      7
#define     COL_545454      8
#define     COL_5454FC      9
#define     COL_54FC54      10
#define     COL_54FCFC      11
#define     COL_FC5454      12
#define     COL_FC54FC      13
#define     COL_FCFC54      14
#define     COL_FCFCFC      15
#define     COL_BLACK       COL_000000
#define     COL_DARKBLUE    COL_0000A8
#define     COL_DARKGREEN   COL_00A800
#define     COL_DARKCYAN    COL_00A8A8
#define     COL_DARKRED     COL_A80000
#define     COL_DARKPURPLE  COL_A800A8
#define     COL_BROWN       COL_A85400
#define     COL_GREY        COL_A8A8A8
#define     COL_DARKGREY    COL_545454
#define     COL_BLUE        COL_5454FC
#define     COL_GREEN       COL_54FC54
#define     COL_CYAN        COL_54FCFC
#define     COL_RED         COL_FC5454
#define     COL_PURPLE      COL_FC54FC
#define     COL_YELLOW      COL_FCFC54
#define     COL_WHITE       COL_FCFCFC


// asm.s
void _load_idt();
void _load_gdt();
void _sti();
void _cli();
void _hlt();
void _stihlt();
int _in8(int port);
int _in16(int port);
int _in32(int port);
void _out8(int port, int data);
void _out16(int port, int data);
void _out32(int port, int data);
void _asm_inthandler_default();
void _asm_inthandler21();

// graphic.c
void draw_pixel(int x, int y, unsigned char color);
void draw_line(int x1, int y1, int x2, int y2, unsigned char color);
void draw_rect(int x, int y, int w, int h, unsigned char color);
void draw_char(int x, int y, char c, unsigned char color);
void draw_text(int x, int y, unsigned char* text, unsigned char color);
void draw_color_test();

// memory.c
void write_mem8(unsigned int addr, unsigned char data);
void hmemset(void *addr, unsigned char c, int size);

// sprintf.c
int sprintf (char *str, const char *fmt, ...);

// descriptor.c
void init_descriptor();

// interrupt.c
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
void init_pic();
void init_pit();
void enable_mouse_keyboard();

// fifo.c
typedef struct
{
    int *buf;       // データ用バッファ
    int size;       // バッファのサイズ
    int pos_r;      // 読み込み位置
    int pos_w;      // 書き込み位置
    int len;        // 書き込み位置 - 読み込み位置
} FIFO32;
void fifo32_init(FIFO32 *fifo, int size, int *buf);
int fifo32_put(FIFO32 *fifo, int data);
int fifo32_get(FIFO32 *fifo);

// keyboard.c
void init_keyboard();
void update_keyboard();
