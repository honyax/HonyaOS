#include "define.h"

WINDOW_MANAGER window_manager;

void init_background(WINDOW* bg);
void win_draw(int x, int y);
void win_draw_window(WINDOW* win);
byte win_get_pixel(WINDOW* win, int x, int y);

void init_window()
{
    // 背景を作成
    WINDOW *bg = (WINDOW *)hmalloc(sizeof(WINDOW));
    WINDOW *fg = (WINDOW *)hmalloc(sizeof(WINDOW));
    bg->x = 0;
    bg->y = 0;
    bg->w = param_screen_x;
    bg->h = param_screen_y;
    bg->pixels = (byte *)hmalloc(param_screen_x * param_screen_y);
    fg->x = 0;
    fg->y = 0;
    fg->w = 0;
    fg->h = 0;
    bg->prev = NULL;
    bg->next = fg;
    fg->prev = bg;
    fg->next = NULL;
    window_manager.background = bg;
    window_manager.foreground = fg;

    init_background(bg);
    win_draw_window(bg);
}

void init_background(WINDOW* bg)
{
    win_draw_rect(bg, 0, 0, bg->w, bg->h, COL_DARKBLUE);
    win_draw_rect(bg, 80, 80, 150, 150, COL_BLUE);
    win_draw_rect(bg, 160, 160, 150, 150, COL_RED);
    win_draw_rect(bg, 240, 240, 150, 150, COL_YELLOW);
}

// 指定したウィンドウを描画する
void win_draw_window(WINDOW* win)
{
    for (int i = win->x; i < win->x + win->w; i++) {
        for (int j = win->y; j < win->y + win->h; j++) {
            win_draw(i, j);
        }
    }
}

// 指定した位置のpixelを描画する
// 全てのウィンドウを調査し、指定位置の最前面を描画する
void win_draw(int x, int y)
{
    // 最前面から調査を開始し、COL_NONE（透明）以外のcolorが見つかった時点でそれを描画する
    WINDOW *win = window_manager.foreground;
    byte color = COL_NONE;
    while (win->prev != NULL) {
        win = win->prev;
        color = win_get_pixel(win, x, y);
        if (color != COL_NONE) {
            break;
        }
    }

    draw_pixel(x, y, color);
}

void update_window()
{
    // TODO: 動いたウィンドウに関してのみ描画する
    return;
}

byte win_get_pixel(WINDOW* win, int x, int y)
{
    short x_min = win->x;
    short x_max = win->x + win->w;
    short y_min = win->y;
    short y_max = win->y + win->h;

    // 指定された位置が範囲外であればNONEを返す
    if (x < x_min || x_max < x || y < y_min || y_max < y) {
        return COL_NONE;
    }

    // 範囲内の場合は当該の位置のpixelを返す
    short x_pos = x - x_min;
    short y_pos = y - y_min;
    return win->pixels[win->w * y_pos + x_pos];
}

void win_draw_pixel(WINDOW* win, int x, int y, byte color)
{
    win->pixels[win->w * y + x] = color;
}

void win_draw_rect(WINDOW* win, int x, int y, int w, int h, byte color)
{
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            win_draw_pixel(win, i, j, color);
        }
    }
}
