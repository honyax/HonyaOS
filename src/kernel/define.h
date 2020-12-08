#pragma once

// define.sとの共通化
#define     BOOT_SIZE               (1024 * 8)      // ブートサイズ
#define     KERNEL_SIZE             (1024 * 8)      // カーネルサイズ

#define     BOOT_LOAD               0x7C00          // ブートプログラムのロード位置
#define     BOOT_END				(BOOT_LOAD + BOOT_SIZE)

#define     PARAM_BASE              0x00101000      // パラメータ領域の開始位置
#define     KERNEL_LOAD             0x00112000      // カーネルのロード位置（プロテクトモード）
#define     KERNEL_PAGE_DIR         0x00200000      // カーネルのページディレクトリ
#define     KERNEL_PAGE_TABLE       0x00201000      // カーネルのページテーブル
#define     HEAP_MEMORY_START       0x00600000      // ヒープメモリの開始位置
#define     HEAP_MEMORY_END         0x07FF0000      // ヒープメモリの終了位置

#define     SECT_SIZE               (512)           // セクタサイズ

#define     BOOT_SECT               (BOOT_SIZE   / SECT_SIZE)   // ブートプログラムのセクタ数
#define     KERNEL_SECT				(KERNEL_SIZE / SECT_SIZE)	// カーネルのセクタ数

#define     E820_RECORD_SIZE		20

#define     PARAM_FONT_ADR          (PARAM_BASE + 0x00000000)   // フォントアドレスの位置
#define     PARAM_SCREEN_X          (PARAM_BASE + 0x0004)       // 画面サイズ（横）の位置
#define     PARAM_SCREEN_Y          (PARAM_BASE + 0x0006)       // 画面サイズ（縦）の位置
#define     PARAM_VRAM              (PARAM_BASE + 0x00000008)   // VRAMの位置

// 共通
#define     NULL            0
#define     TRUE            1
#define     FALSE           0
typedef     unsigned char   bool;
typedef     unsigned char   byte;
typedef     unsigned short  ushort;
typedef     unsigned int    uint;

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
#define     COL_NONE        255

// Global Segment Descriptor定義値
#define GDT_IDX_NULL        0
#define GDT_IDX_CODE        1
#define GDT_IDX_DATA        2
#define GDT_IDX_LDT         3
#define GDT_IDX_ROOT_TSS    4

// Local Segment Descriptor定義値
#define LDT_IDX_NULL        0
#define LDT_IDX_ROOT_CS     1
#define LDT_IDX_ROOT_DS     2

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

// keyboard
#define PORT_KEYDAT             0x60

// fifo
typedef struct
{
    int *buf;       // データ用バッファ
    int size;       // バッファのサイズ
    int pos_r;      // 読み込み位置
    int pos_w;      // 書き込み位置
    int len;        // 書き込み位置 - 読み込み位置
} FIFO32;

// task
typedef struct
{
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
} TSS;

// window
typedef struct window
{
    short x, y;
    short w, h;
    byte *pixels;
    struct window *prev;
    struct window *next;
} WINDOW;

typedef struct
{
    WINDOW *background;      // 背景
    WINDOW *foreground;      // 前景（ダミーのシート。番兵）
} WINDOW_MANAGER;

// kernel.c
extern byte* param_font_adr;
extern ushort param_screen_x;
extern ushort param_screen_y;
extern uint param_vram;
extern uint _text_start;
extern uint _text_end;
extern uint _rodata_start;
extern uint _rodata_end;
extern uint _data_start;
extern uint _data_end;
extern uint _bss_start;
extern uint _bss_end;

// asm.s
void _load_idt();
void _load_gdt();
void _sti();
void _cli();
void _hlt();
void _stihlt();
void _load_tr(int tr);
void _farjmp(int eip, short cs);
uint _get_cr0();
void _set_cr0(uint value);
void _set_cr3(uint value);
int _in8(int port);
int _in16(int port);
int _in32(int port);
void _out8(int port, int data);
void _out16(int port, int data);
void _out32(int port, int data);
void _asm_inthandler_default();
void _asm_inthandler00();
void _asm_inthandler01();
void _asm_inthandler05();
void _asm_inthandler06();
void _asm_inthandler07();
void _asm_inthandler0a();
void _asm_inthandler0b();
void _asm_inthandler0c();
void _asm_inthandler0d();
void _asm_inthandler0e();
void _asm_inthandler10();
void _asm_inthandler11();
void _asm_inthandler20();
void _asm_inthandler21();
void _asm_inthandler2c();
void _asm_syscall();
void _magic_break();

// asm_syscall.s
int _sc_example(int param1, int param2, int param3, int param4, int param5);
void _sc_putchar(int c);

// graphic.c
void draw_pixel(int x, int y, byte color);
byte get_pixel(int x, int y);
void draw_line(int x1, int y1, int x2, int y2, byte color);
void draw_rect(int x, int y, int w, int h, byte color);
void draw_char(int x, int y, char c, byte color);
void draw_text(int x, int y, byte* text, byte color);

// memory.c
void write_mem8(uint addr, byte data);
byte read_mem8(uint addr);
void hmemset(void *addr, byte c, int size);
void init_memory();
void *hmalloc(uint size);
void hfree(void *ptr);

// sprintf.c
int hsprintf (char *str, const char *fmt, ...);

// descriptor.c
void init_descriptor();
void create_root_task(TSS *task);
void create_task(TSS *task, int task_index);

// interrupt.c
void init_pic();
void init_pit();
void enable_mouse_keyboard();
void update_interrupt();

// fifo.c
void fifo32_init(FIFO32 *fifo, int size, int *buf);
int fifo32_put(FIFO32 *fifo, int data);
int fifo32_get(FIFO32 *fifo);

// keyboard.c
void init_keyboard();
bool try_get_key_input(char *c);

// mouse.c
void init_mouse();
int update_mouse();
bool isLeftButtonPushed();
void getMousePos(int *x, int *y);

// timer.c
void init_timer();
void update_timer();

// paging.c
void init_paging();

// task.c
void init_task();
void task_switch();

// window.c
void init_window();
WINDOW* win_create(int x, int y, int w, int h);
void win_move(WINDOW* win, int x, int y);
byte win_get_pixel(WINDOW* win, int x, int y);
void win_draw_pixel(WINDOW* win, int x, int y, byte color);
void win_draw_line(WINDOW* win, int x1, int y1, int x2, int y2, byte color);
void win_draw_rect(WINDOW* win, int x, int y, int w, int h, byte color);
void win_draw_char(WINDOW* win, int x, int y, char c, byte color);
void win_draw_text(WINDOW* win, int x, int y, byte* text, byte color);
void bg_draw_pixel(int x, int y, byte color);
void bg_draw_line(int x1, int y1, int x2, int y2, byte color);
void bg_draw_rect(int x, int y, int w, int h, byte color);
void bg_draw_char(int x, int y, char c, byte color);
void bg_draw_text(int x, int y, byte* text, byte color);

// console.c
void init_console();
void update_console();
