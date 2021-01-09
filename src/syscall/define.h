#pragma once

#include "../lib/define.h"

#define SYSCALL_ID_EXAMPLE              0xFFFF
#define SYSCALL_ID_BG_DRAW_TEXT         0x100
#define SYSCALL_ID_GET_CURRENT_TIME     0x200
#define SYSCALL_ID_SLEEP                0x201
#define SYSCALL_ID_GET_MOUSE_POS        0x202
#define SYSCALL_ID_WIN_CREATE           0x300
#define SYSCALL_ID_WIN_DRAW_LINE        0x301
#define SYSCALL_ID_WIN_DRAW_RECT        0x302
#define SYSCALL_ID_WIN_DRAW_BYTES       0x303
#define SYSCALL_ID_WIN_DRAW_TEXT        0x304

// asm_syscall.s
int _sc_example(int param1, int param2, int param3, int param4, int param5);
void _sc_bg_draw_text(int x, int y, char *text, int color);
int _sc_get_current_time();
void _sc_sleep(int milliseconds);
int _sc_get_mouse_pos();
int _sc_win_create(RECT *rect);
void _sc_win_draw_line(int win_handle, VECTOR *src, VECTOR *dst, int color);
void _sc_win_draw_rect(int win_handle, RECT *rect, int color);
void _sc_win_draw_bytes(int win_handle, RECT *rect, byte *data);
void _sc_win_draw_text(int win_handle, int x, int y, char *text, int color);
