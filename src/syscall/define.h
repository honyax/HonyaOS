#pragma once

#include "../lib/define.h"

#define SYSCALL_ID_EXAMPLE              0xFFFF
#define SYSCALL_ID_PUTCHAR              1
#define SYSCALL_ID_BG_DRAW_TEXT         2
#define SYSCALL_ID_GET_CURRENT_TIME     3
#define SYSCALL_ID_SLEEP                4
#define SYSCALL_ID_WIN_CREATE           5
#define SYSCALL_ID_WIN_DRAW_RECT        6
#define SYSCALL_ID_WIN_DRAW_TEXT        7
#define SYSCALL_ID_WIN_DRAW_BYTES       8

// asm_syscall.s
int _sc_example(int param1, int param2, int param3, int param4, int param5);
void _sc_putchar(int c);
void _sc_bg_draw_text(int x, int y, char *text, int color);
int _sc_get_current_time();
void _sc_sleep(int milliseconds);
int _sc_win_create(RECT *rect);
void _sc_win_draw_rect(int win_handle, RECT *rect, int color);
void _sc_win_draw_text(int win_handle, int x, int y, char *text, int color);
void _sc_win_draw_bytes(int win_handle, RECT *rect, byte *data);
