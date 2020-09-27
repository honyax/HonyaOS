#pragma once

// define.sとの共通化
#define     BOOT_SIZE               (1024 * 8)      // ブートサイズ
#define     KERNEL_SIZE             (1024 * 8)      // カーネルサイズ

#define     BOOT_LOAD               0x7C00          // ブートプログラムのロード位置
#define     BOOT_END				(BOOT_LOAD + BOOT_SIZE)

#define     PARAM_BASE              0x00101000      // パラメータ領域の開始位置
#define     ERNEL_LOAD              0x0010_3000     // カーネルのロード位置（プロテクトモード）

#define     SECT_SIZE               (512)           // セクタサイズ

#define     BOOT_SECT               (BOOT_SIZE   / SECT_SIZE)   // ブートプログラムのセクタ数
#define     KERNEL_SECT				(KERNEL_SIZE / SECT_SIZE)	// カーネルのセクタ数

#define     E820_RECORD_SIZE		20

#define     PARAM_FONT_ADR          PARAM_BASE      // フォントアドレスの位置



void fuga(int);
int hemo(int, int, int, int, int, int);
void draw_char(int col, int row, int color, char ch);
int asm_sample();
