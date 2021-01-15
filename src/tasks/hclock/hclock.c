#include "../../syscall/define.h"
#include "../../lib/define.h"

#define BOARD_RADIUS    80          // 時計の半径
#define OUTER_RADIUS    75          // 外側の半径
#define INNER_RADIUS    70          // 内側の半径
#define MINUTE_LENGTH   65          // 長針の長さ
#define HOUR_LENGTH     40          // 短針の長さ

// 時計盤の中心からのXY座標を、ウィンドウのXY座標に変換する
void convert_to_window_pos(int x, int y, int *win_x, int *win_y)
{
    *win_x = x + BOARD_RADIUS;
    *win_y = -y + BOARD_RADIUS;
}

// 時計の目盛りを描画
void draw_dial(int win_handle, int x1, int y1, int x2, int y2)
{
    VECTOR src, dst;
    convert_to_window_pos(x1, y1, &src.x, &src.y);
    convert_to_window_pos(x2, y2, &dst.x, &dst.y);
    _sc_win_draw_line(win_handle, &src, &dst, COL_WHITE);
}

// radiusの長さにおける指定したminuteの位置を返す。minuteの範囲は0～59
void get_position(int minute, int *x, int *y, int radius)
{
    int r0 = radius;
    int r1 = radius / 2;
    int r2 = hsqrt(radius * radius * 3 / 4);
    int x_ar[12] = {0, r1, r2, r0, r2, r1, 0, -r1, -r2, -r0, -r2, -r1};
    int y_ar[12] = {r0, r2, r1, 0, -r1, -r2, -r0, -r2, -r1, 0, r1, r2};
    // まずは12分割した位置を取得
    int x_base = x_ar[minute / 5];
    int y_base = y_ar[minute / 5];

    // 次の位置との間で適当に補間する（ここは適当）
    int x_next, y_next;
    if ((minute / 5) == 11) {
        x_next = x_ar[0];
        y_next = y_ar[0];
    } else {
        x_next = x_ar[minute / 5 + 1];
        y_next = y_ar[minute / 5 + 1];
    }
    int p = minute % 5;
    *x = (x_base * (5 - p) + x_next * p) / 5;
    *y = (y_base * (5 - p) + y_next * p) / 5;
}

// 長針を描画
void draw_minute_hand(int win_handle, int minute)
{
    int x, y;
    get_position(minute, &x, &y, MINUTE_LENGTH);
    draw_dial(win_handle, 0, 0, x, y);
}

// 短針を描画
void draw_hour_hand(int win_handle, int hour, int minute)
{
    // hour, minuteから、短針が長針で言う何分の位置にあるかを算出
    // 12時間表示
    hour %= 12;
    int val = hour * 5 + minute / 12;
    int x, y;
    get_position(val, &x, &y, HOUR_LENGTH);
    draw_dial(win_handle, 0, 0, x, y);
}

// 時計盤を描画
void draw_board(int win_handle)
{
    // 時計盤の描画
    RECT rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = BOARD_RADIUS * 2;
    rect.h = BOARD_RADIUS * 2;
    _sc_win_draw_rect(win_handle, &rect, COL_DARKGREEN);

    // 目盛りの描画
    for (int i = 0; i < 12; i++) {
        int min = i * 5;
        int outer_x, outer_y;
        int inner_x, inner_y;
        get_position(min, &outer_x, &outer_y, OUTER_RADIUS);
        get_position(min, &inner_x, &inner_y, INNER_RADIUS);
        draw_dial(win_handle, outer_x, outer_y, inner_x, inner_y);
    }
}

void main()
{
    RECT rect;
    rect.x = BOARD_RADIUS * 2;
    rect.y = BOARD_RADIUS * 2;
    rect.w = BOARD_RADIUS * 2;
    rect.h = BOARD_RADIUS * 2;
    int win_handle = _sc_win_create(&rect);

    int last_hhmm00;
    for (;;) {
        // 1秒毎に必要であれば再描画
        int hhmm00 = _sc_get_current_time() & 0xFFFF00;
        if (last_hhmm00 != hhmm00) {
            last_hhmm00 = hhmm00;
            int hour = (hhmm00 & 0xFF0000) >> 16;
            hour = hour / 0x10 * 10 + hour % 0x10;
            int min = (hhmm00 & 0xFF00) >> 8;
            min = min / 0x10 * 10 + min % 0x10;

            // 時計盤を描画
            draw_board(win_handle);

            // 長針、短針を描画
            draw_minute_hand(win_handle, min);
            draw_hour_hand(win_handle, hour, min);
        }
        _sc_sleep(1000);
    }
}
