#include "define.h"

static WINDOW* console_win;

typedef struct
{
    char text[2560];
    int win_start_pos;
    int cmd_start_pos;
} CONSOLE;
static CONSOLE cons_data;

void init_console()
{
    int w = param_screen_x / 2;
    int h = param_screen_y / 2;
    int x = param_screen_x / 2 - (w / 2);
    int y = param_screen_y / 2 - (h  / 2);
    console_win = win_create(x, y, w, h);
    win_draw_rect(console_win, 0, 0, w, h, COL_BLACK);

    cons_data.text[0] = '>';
    cons_data.win_start_pos = 0;
    cons_data.cmd_start_pos = 1;
    win_draw_text(console_win, 0, 0, cons_data.text, COL_WHITE);
}

void update_console()
{
    if (isLeftButtonPushed()) {
        int x, y;
        getMousePos(&x, &y);
        win_move(console_win, x, y);
    }
}
