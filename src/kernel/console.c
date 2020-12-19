#include "define.h"

#define TEXT_LENGTH 40  // 1280 / 4 / 8  = 40
#define LINE_COUNT 8    // 1024 / 8 / 16 = 8

static WINDOW *console_win;

typedef struct
{
    char text[LINE_COUNT][TEXT_LENGTH];
    int current_line;
    int current_pos;
    int display_pos;
} CONSOLE;
static CONSOLE cons_data;

typedef struct
{
    byte name[8];
    byte ext[3];
    byte type;
    char reserve[10];
    ushort time;
    ushort date;
    ushort clustno;
    uint size;
} FILEINFO;
static FILEINFO *file_info;
static int max_file_count;

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

void println(const char *str)
{
    print(str);
    add_newline();
}

void init_console()
{
    int w = param_screen_x / 4;
    int h = param_screen_y / 4;
    int x = param_screen_x / 2 - (w / 2);
    int y = param_screen_y / 2 - (h  / 2);
    console_win = win_create(x, y, w, h);
    win_draw_rect(console_win, 0, 0, w, h, COL_BLACK);

    cons_data.text[0][0] = '>';
    cons_data.current_line = 0;
    cons_data.current_pos = 1;
    cons_data.display_pos = 0;

    // ファイルシステムヘッダの解析
    byte* fs_pos = (byte*) FILESYSTEM_LOAD;

    ushort fat_tbl_head_sector = fs_pos[0x0f] << 8 | fs_pos[0x0e];
    byte fat_tbl_num = fs_pos[0x10];
    ushort sector_per_fat_tbl = fs_pos[0x17] << 8 | fs_pos[0x16];
    max_file_count = fs_pos[0x12] << 8 | fs_pos[0x11];

#if 0
    // FATテーブルの領域
    char str[64];
    hsprintf(str, "%d, %d, %d, %d", fat_tbl_head_sector, fat_tbl_num, sector_per_fat_tbl, max_file_count);
    println(str);
#endif
    // FATテーブルをスキップした先頭が、ファイルの先頭
    file_info = (FILEINFO *) &(fs_pos[SECT_SIZE * fat_tbl_head_sector + SECT_SIZE * sector_per_fat_tbl * fat_tbl_num]);
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
    } else if (cmd_equals(input, "mem")) {
        // メモリ使用状況を獲得
        int total, used, free, count;
        char str[64];
        get_memory_status(&total, &used, &free, &count);
        hsprintf(str, "%d, %d, %d, %d", total, used, free, count);
        println("total, used, free, count");
        println(str);
    } else if (cmd_equals(input, "ls")) {
        // ファイルリストを表示
        
        char fname[16];
        int fname_index = 0;
        for (int i = 0; i < max_file_count; i++) {
            // nameの先頭が 0x00 の場合はそこで終了
            if (file_info[i].name[0] == 0x00)
                break;
            // フィアルの場合はタイプが 0x20
            if (file_info[i].type != 0x20)
                continue;
            
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
            fname[fname_index] = 0;
            println(fname);
        }
    } else {
        println("Bad Command");
    }
}
