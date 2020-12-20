#include "define.h"

#define CONSOLE_WIDTH 480
#define CONSOLE_HEIGHT 320
#define TEXT_LENGTH 60  // 480 / 8  = 60
#define LINE_COUNT 20    // 320 / 16 = 20

static WINDOW *console_win;

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
void exec_clear();
void exec_mem();
void exec_ls();
void exec_cat(char *filename);

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

void println(const char *str)
{
    print(str);
    add_newline();
}

void init_console()
{
    int w = CONSOLE_WIDTH;
    int h = CONSOLE_HEIGHT;
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
    int w = CONSOLE_WIDTH;
    int h = CONSOLE_HEIGHT;
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

char *get_cmd_arg(char *input, const char *cmd)
{
    if (!cmd_equals(input, cmd))
        return NULL;

    int cmd_len = hstrlen(cmd);
    int input_len = hstrlen(input);
    if (cmd_len + 1 < input_len) {
        return &input[cmd_len + 1];
    } else {
        return NULL;
    }
}

void exec_command(char *input)
{
    if (cmd_equals(input, "")) {
        // 改行のみの場合は無処理
    } else if (cmd_equals(input, "clear")) {
        exec_clear();
    } else if (cmd_equals(input, "mem")) {
        exec_mem();
    } else if (cmd_equals(input, "ls")) {
        exec_ls();
    } else if (cmd_equals(input, "cat")) {
        exec_cat(get_cmd_arg(input, "cat"));
    } else {
        println("Bad Command");
    }
}

void exec_clear()
{
    // 画面をクリアする
    for (int line = 0; line < LINE_COUNT; line++) {
        for (int i = 0; i < TEXT_LENGTH; i++) {
            cons_data.text[line][i] = 0;
        }
    }
    cons_data.current_line = 0;
    refresh_console();
}

void exec_mem()
{
    // メモリ使用状況を獲得
    int total, used, free, count;
    char str[64];
    get_memory_status(&total, &used, &free, &count);
    hsprintf(str, "%d, %d, %d, %d", total, used, free, count);
    println("total, used, free, count");
    println(str);
}

void exec_ls()
{
    // ファイルリストを表示
    
    // 表示フォーマット
    // ファイル名    時刻             サイズ     クラスタ番号
    // FILENAME.EXT 2020/12/31 23:59 9999999999 99999
    println("FileName     DateTime(UTC)    Size       ClustNo");

    FILEINFO *file_info = get_file_info();
    int file_count = get_file_count();
    char fname[64];
    int fname_index = 0;
    for (int i = 0; i < file_count; i++) {
        // ファイルの場合はタイプが 0x20
        if (file_info[i].type != 0x20)
            continue;
        
        for (int j = 0; j < 45; j++) {
            fname[j] = ' ';
        }

        // ファイル名を出力
        fname_index = 0;
        for (int j = 0; j < 8; j++) {
            if (file_info[i].name[j] != 0x20) {
                fname[fname_index++] = file_info[i].name[j];
            }
        }
        if (file_info[i].ext[0] != 0x20) {
            fname[fname_index++] = '.';
            for (int j = 0; j < 3; j++) {
                fname[fname_index++] = file_info[i].ext[j];
            }
        }

        // yearに加算するのはネットの情報によると1980らしいが、手元では2010にしないとダメっぽい。謎。
        int year = ((file_info[i].update_date & 0xf800) >> 11) + 2010;
        int mon  = (file_info[i].update_date & 0x07e0) >>  5;
        int day  = (file_info[i].update_date & 0x001f) >>  0;
        int hour = (file_info[i].update_time & 0xf800) >> 11;
        int min  = (file_info[i].update_time & 0x07e0) >>  5;
        int count;
        // hsprintfで最後にnullを追加されるので、それを無理やり半角空白で上書きするという苦肉の策。%2dみたいに長さ指定できるようにしたい。。。
        count = hsprintf(&fname[13], "%d/%d/%d", year, mon, day);
        fname[13 + count] = ' ';
        count = hsprintf(&fname[24], "%d:%d", hour, min);
        fname[24 + count] = ' ';
        count = hsprintf(&fname[30], "%d", file_info[i].size);
        fname[30 + count] = ' ';
        // ファイルシステムが320KBなので、クラスタ番号の上位2バイトを考慮する必要ない
        hsprintf(&fname[41], "%d", file_info[i].clustno_lo);
        println(fname);
    }
}

void exec_cat(char *filename)
{
    if (filename == NULL) {
        println("Invalid filename");
        return;
    }
    char str[32];
    hsprintf(str, "len:%d %s", hstrlen(filename), filename);
    println(str);
}
