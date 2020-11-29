#include "define.h"

// カラーテストを行う場合は以下をTRUEにする
#define DRAW_COLOR_TEST FALSE

WINDOW_MANAGER window_manager;

void init_background(WINDOW* bg);
void screen_draw_window(WINDOW* win);
void screen_draw_rect(int x, int y, int w, int h);
void screen_draw_pixel(int x, int y);

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
    screen_draw_window(bg);
}

void init_background(WINDOW* bg)
{
#if !DRAW_COLOR_TEST

    win_draw_rect(bg, 0, 0, bg->w, bg->h, COL_DARKBLUE);
    win_draw_rect(bg, 80, 80, 150, 150, COL_BLUE);
    win_draw_rect(bg, 160, 160, 150, 150, COL_RED);
    win_draw_rect(bg, 240, 240, 150, 150, COL_YELLOW);
    win_draw_line(bg, 100, 200, 1000, 800, COL_GREEN);
    win_draw_char(bg, 16, 80, 'X', COL_WHITE);
    win_draw_char(bg, 24, 80, 'Y', COL_GREY);
    win_draw_char(bg, 32, 80, 'Z', COL_CYAN);
    win_draw_text(bg, 16, 120, "HonyaOS is my own operating system.", COL_WHITE);

    char test_txt[128];
    hsprintf(test_txt, "This is %d, 0x%x, 0x%X, string:%s.", 100, 0x12AB, 0x34CD, "string parameter");
    win_draw_text(bg, 16, 160, test_txt, COL_YELLOW);

    uint _TEXT_START    = ( uint )&_text_start;
    uint _TEXT_END      = ( uint )&_text_end;
    uint _RODATA_START  = ( uint )&_rodata_start;
    uint _RODATA_END    = ( uint )&_rodata_end;
    uint _DATA_START    = ( uint )&_data_start;
    uint _DATA_END      = ( uint )&_data_end;
    uint _BSS_START     = ( uint )&_bss_start;
    uint _BSS_END       = ( uint )&_bss_end;
    hsprintf(test_txt, "text:%X - %X rodata:%X - %X data:%X - %X bss:%X - %X",
            _TEXT_START, _TEXT_END, _RODATA_START, _RODATA_END, _DATA_START, _DATA_END, _BSS_START, _BSS_END);
    win_draw_text(bg, 16, 480, test_txt, COL_CYAN);

#else

    // カラーテスト
    int dx = param_screen_x / 16;
    int dy = param_screen_y / 16;

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            byte color = y * 16 + x;
            win_draw_rect(bg, dx * x, dy * y, dx, dy, color);
            char str[5];
            hsprintf(str, "0x%X%X", y, x);
            byte text_color;
            if ((x < 10 && y < 3) ||
                (color >= 0x36 && color <= 0x39) ||
                color > 0x67) {
                text_color = COL_FCFCFC;
            } else {
                text_color = COL_000000;
            }
            win_draw_text(bg, dx * x + (dx / 2 - 16), dy * y + (dy / 2 - 8), str, text_color);
        }
    }

#endif
}

// 指定したウィンドウを描画する
void screen_draw_window(WINDOW* win)
{
    screen_draw_rect(win->x, win->y, win->w, win->h);
}

void screen_draw_rect(int x, int y, int w, int h)
{
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            screen_draw_pixel(i, j);
        }
    }
}

// 指定した位置のpixelを描画する
// 全てのウィンドウを調査し、指定位置の最前面を描画する
void screen_draw_pixel(int x, int y)
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

void win_draw_pixel(WINDOW* win, int x, int y, byte color)
{
    win->pixels[win->w * y + x] = color;
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

void win_draw_line(WINDOW* win, int x1, int y1, int x2, int y2, byte color)
{
    int delta_x = x1 > x2 ? x1 - x2 : x2 - x1;
    int delta_y = y1 > y2 ? y1 - y2 : y2 - y1;
    int delta = delta_x > delta_y ? delta_x : delta_y;
    int diff_x = x2 - x1;
    int diff_y = y2 - y1;
    for (int i = 0; i <= delta; i++) {
        int x = x1 + diff_x * i / delta;
        int y = y1 + diff_y * i / delta;
        if (0 <= x && x < win->w && 0 <= y && y < win->y) {
            win_draw_pixel(win, x, y, color);
        }
    }
}

void win_draw_rect(WINDOW* win, int x, int y, int w, int h, byte color)
{
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            win_draw_pixel(win, i, j, color);
        }
    }
}

void win_draw_char(WINDOW* win, int x, int y, char c, byte color)
{
    int index = c * 16;
    for (int i = 0; i < 16; i++) {
        byte data = param_font_adr[index + i];
        if ((data & 0x80) != 0) { win_draw_pixel(win, x + 0, y + i, color); }
        if ((data & 0x40) != 0) { win_draw_pixel(win, x + 1, y + i, color); }
        if ((data & 0x20) != 0) { win_draw_pixel(win, x + 2, y + i, color); }
        if ((data & 0x10) != 0) { win_draw_pixel(win, x + 3, y + i, color); }
        if ((data & 0x08) != 0) { win_draw_pixel(win, x + 4, y + i, color); }
        if ((data & 0x04) != 0) { win_draw_pixel(win, x + 5, y + i, color); }
        if ((data & 0x02) != 0) { win_draw_pixel(win, x + 6, y + i, color); }
        if ((data & 0x01) != 0) { win_draw_pixel(win, x + 7, y + i, color); }
    }
}

void win_draw_text(WINDOW* win, int x, int y, byte* text, byte color)
{
    for (int i = 0; *(text + i) != 0x00; i++) {
        win_draw_char(win, x + 8 * i, y, text[i], color);
    }
}
