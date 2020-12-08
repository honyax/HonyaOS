#include "define.h"

#define TEXT_LENGTH 320     // (1280 / 4 / 8) * (1024 / 8 / 16) = 40 * 8 = 320

static WINDOW* console_win;

typedef struct
{
    char text[TEXT_LENGTH];
    int win_start_pos;
    int cmd_start_pos;
    int current_pos;
    int display_pos;
} CONSOLE;
static CONSOLE cons_data;

void add_key(char c)
{
    cons_data.text[cons_data.current_pos] = c;
    cons_data.current_pos++;
    if (cons_data.current_pos >= TEXT_LENGTH) {
        cons_data.current_pos = 0;
    }
}

void init_console()
{
    int w = param_screen_x / 4;
    int h = param_screen_y / 8;
    int x = param_screen_x / 2 - (w / 2);
    int y = param_screen_y / 2 - (h  / 2);
    console_win = win_create(x, y, w, h);
    win_draw_rect(console_win, 0, 0, w, h, COL_BLACK);

    cons_data.text[0] = '>';
    cons_data.win_start_pos = 0;
    cons_data.cmd_start_pos = 1;
    cons_data.current_pos = 1;
    cons_data.display_pos = 0;
}

void update_console()
{
    if (isLeftButtonPushed()) {
        int x, y;
        getMousePos(&x, &y);
        win_move(console_win, x, y);
    }
    // 同時に複数キー入力される可能性を考慮して、最大8ループさせる
    char c;
    for (int i = 0; i < 8; i++) {
        if (!try_get_key_input(&c))
            break;
        
        add_key(c);
    }

    if (cons_data.current_pos != cons_data.display_pos) {
        win_draw_text(console_win, 8 * cons_data.display_pos, 0, &cons_data.text[cons_data.display_pos], COL_WHITE);
        cons_data.display_pos = cons_data.current_pos;
    }
}
