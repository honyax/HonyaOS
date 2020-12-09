#include "define.h"

#define TEXT_LENGTH 40  // 1280 / 4 / 8  = 40
#define LINE_COUNT 8    // 1024 / 8 / 16 = 8

static WINDOW* console_win;

typedef struct
{
    char text[LINE_COUNT][TEXT_LENGTH];
    int current_line;
    int current_pos;
    int display_pos;
} CONSOLE;
static CONSOLE cons_data;

void refresh_console();
void exec_command(char *line);

void print(const char *str)
{
    int len = hstrlen(str);
    for (int i = 0; i < len; i++) {
        cons_data.text[cons_data.current_line][cons_data.current_pos + i] = str[i];
    }
    win_draw_text(console_win,
        8 * cons_data.current_pos, 16 * cons_data.current_line,
        &cons_data.text[cons_data.current_line][cons_data.current_pos], COL_WHITE);
}

// 改行処理を実行
void add_newline()
{
    // まだ最終行に到達していない場合は行を一つ加算するのみ
    if (cons_data.current_line + 1 < LINE_COUNT) {
        cons_data.current_line++;
    } else {
        // 既に最下段に達しているので、全ての行を一つずつ上げる。最終行だけは0クリアする
        for (int line = 0; line < LINE_COUNT; line++) {
            for (int i = 0; i < TEXT_LENGTH; i++) {
                cons_data.text[line][i] = (line == LINE_COUNT - 1) ? 0 : cons_data.text[line + 1][i];
            }
        }
        refresh_console();
    }
    cons_data.current_pos = 0;
    cons_data.display_pos = 0;
}

void add_key(char c)
{
    switch (c) {
        case 0x00:
            break;
        case 0x08:  // BackSpace
            if (cons_data.current_pos > 1) {
                cons_data.current_pos--;
                cons_data.text[cons_data.current_line][cons_data.current_pos] = 0;
            }
            break;
        case '\n':  // Enter
            add_newline();

            // 入力されたコマンドを解析、実行
            exec_command(&cons_data.text[cons_data.current_line - 1][1]);

            cons_data.text[cons_data.current_line][0] = '>';
            cons_data.current_pos = 1;
            break;
        default:
            if (cons_data.current_pos < TEXT_LENGTH) {
                cons_data.text[cons_data.current_line][cons_data.current_pos] = c;
                cons_data.current_pos++;
            }
            break;
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

    cons_data.text[0][0] = '>';
    cons_data.current_line = 0;
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

    if (cons_data.current_pos > cons_data.display_pos) {
        win_draw_text(console_win,
            8 * cons_data.display_pos, 16 * cons_data.current_line,
            &cons_data.text[cons_data.current_line][cons_data.display_pos], COL_WHITE);
    } else if (cons_data.current_pos < cons_data.display_pos) {
        win_draw_rect(console_win,
            8 * cons_data.current_pos, 16 * cons_data.current_line,
            8 * cons_data.display_pos - cons_data.current_pos, 16, COL_BLACK);
    }
    cons_data.display_pos = cons_data.current_pos;
}

void refresh_console()
{
    int w = param_screen_x / 4;
    int h = param_screen_y / 8;
    win_draw_rect(console_win, 0, 0, w, h, COL_BLACK);
    for (int i = 0; i <= cons_data.current_line; i++) {
        win_draw_text(console_win, 0, 16 * i, &cons_data.text[i][0], COL_WHITE);
    }
}

bool cmd_equals(char *input, const char *cmd)
{
    int len = hstrlen(cmd);
    if (len < 0)
        return FALSE;
    
    for (int i = 0; i < len; i++) {
        if (input[i] != cmd[i])
            return FALSE;
    }

    return input[len] == '\n' || input[len] == ' ' || input[len] == 0;
}

void exec_command(char *input)
{
    if (cmd_equals(input, "")) {
        // 改行のみの場合は無処理
    } else if (cmd_equals(input, "clear")) {
        // 画面をクリアする
        for (int line = 0; line < LINE_COUNT; line++) {
            for (int i = 0; i < TEXT_LENGTH; i++) {
                cons_data.text[line][i] = 0;
            }
        }
        cons_data.current_line = 0;
        refresh_console();
    } else {
        print("Bad Command");
        add_newline();
    }
}
