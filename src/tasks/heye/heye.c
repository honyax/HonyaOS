#include "../../syscall/define.h"
#include "../../lib/define.h"

#define RADIUS          64          // 目の半径のサイズ
#define WIDTH           8           // 目の枠の太さ
#define RANGE           45          // 瞳が移動できる距離
#define PUPIL_RADIUS    8           // 瞳の半径

void main()
{
    // 目の描画
    int x, y;
    int index;
    byte eye_data[RADIUS * 2 * RADIUS * 2];
    for (int i = 0; i < RADIUS * 2; i++) {
        for (int j = 0; j < RADIUS * 2; j++) {
            index = j * RADIUS * 2 + i;
            x = i - RADIUS;
            y = j - RADIUS;
            if (RADIUS * RADIUS < x * x + y * y) {
                eye_data[index] = COL_NONE;
            } else if ((RADIUS - WIDTH) * (RADIUS - WIDTH) < x * x + y * y) {
                eye_data[index] = COL_BLACK;
            } else {
                eye_data[index] = COL_WHITE;
            }
        }
    }

    byte pupil_data[PUPIL_RADIUS * 2 * PUPIL_RADIUS * 2];
    for (int i = 0; i < PUPIL_RADIUS * 2; i++) {
        for (int j = 0; j < PUPIL_RADIUS * 2; j++) {
            index = j * PUPIL_RADIUS * 2 + i;
            x = i - PUPIL_RADIUS;
            y = j - PUPIL_RADIUS;
            if (PUPIL_RADIUS * PUPIL_RADIUS < x * x + y * y) {
                pupil_data[index] = COL_WHITE;
            } else {
                pupil_data[index] = COL_BLACK;
            }
        }
    }

    RECT rect;
    rect.x = RADIUS * 2;
    rect.y = RADIUS * 2;
    rect.w = RADIUS * 2;
    rect.h = RADIUS * 2;
    int win_handle = _sc_win_create(&rect);
    rect.x = 0;
    rect.y = 0;
    _sc_win_draw_bytes(win_handle, &rect, eye_data);

    // TODO: win_handleがWINDOWのポインタになっており、先頭からウィンドウのX座標、Y座標が2byteずつで格納されている。
    // ここでは一旦これを使う。
    short *win_pos = (short *) win_handle;

    int mouse_pos;
    int center_pos_x, center_pos_y;
    int mouse_pos_x, mouse_pos_y;
    int diff_x, diff_y;
    int last_x, last_y;
    int cur_x, cur_y;
    int sqrlen, len;
    rect.x = RADIUS;
    rect.y = RADIUS;
    rect.w = PUPIL_RADIUS * 2;
    rect.h = PUPIL_RADIUS * 2;

    for (;;) {
        mouse_pos = _sc_get_mouse_pos();
        center_pos_x = win_pos[0] + RADIUS;
        center_pos_y = win_pos[1] + RADIUS;
        mouse_pos_x = (mouse_pos >> 16) & 0xFFFF;
        mouse_pos_y = mouse_pos & 0xFFFF;
        diff_x = mouse_pos_x - center_pos_x;
        diff_y = mouse_pos_y - center_pos_y;
        cur_x = mouse_pos_x - win_pos[0] - PUPIL_RADIUS;
        cur_y = mouse_pos_y - win_pos[1] - PUPIL_RADIUS;

        if (cur_x != last_x || cur_y != last_y) {
            // 直前に描画した瞳を消す（デフォルトは中心なので問題ない）
            _sc_win_draw_rect(win_handle, &rect, COL_WHITE);

            sqrlen = diff_x * diff_x + diff_y * diff_y;

            if (sqrlen < RANGE * RANGE) {
                // 目の範囲内の場合はマウス位置に瞳を描画
                rect.x = cur_x;
                rect.y = cur_y;
            } else {
                // 目の範囲外の場合はマウスの方向に瞳を描画
                len = hsqrt(sqrlen);
                rect.x = diff_x * RANGE / len + RADIUS - WIDTH;
                rect.y = diff_y * RANGE / len + RADIUS - WIDTH;
            }
            _sc_win_draw_bytes(win_handle, &rect, pupil_data);
            last_x = cur_x;
            last_y = cur_y;
        }

        _sc_sleep(100);
    }
}
