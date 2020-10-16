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

#define     VBE_MODE                (0x107)         // 1280 x 1024 x 8bitカラー
#define     SCREEN_X                (1280)          // 画面サイズ（横）
#define     SCREEN_Y                (1024)          // 画面サイズ（縦）
//#define   VBE_MODE                (0x105)         // 1024 x 768 x 8bitカラー
//#define   SCREEN_X                (1024)          // 画面サイズ（横）
//#define   SCREEN_Y                (768)           // 画面サイズ（縦）
#define     VRAM                    (0xe0000000)    // VRAMのアドレス

#define     PARAM_FONT_ADR          PARAM_BASE      // フォントアドレスの位置



int _asm_sample();

void draw_pixel(int x, int y, unsigned char color);
void draw_line(int x1, int y1, int x2, int y2, unsigned char color);
void draw_rect(int x, int y, int w, int h, unsigned char color);
void draw_char(int x, int y, char c, unsigned char color);
void draw_text(int x, int y, unsigned char* text, unsigned char color);

void write_mem8(unsigned int addr, unsigned char data);
void hmemset(void *addr, unsigned char c, int size);

int sprintf (char *str, const char *fmt, ...);