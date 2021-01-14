#include "define.h"

// 各システムコールの処理
void syscall(int sc_id, int param1, int param2, int param3, int param4, int param5)
{
    // アプリケーションに渡す戻り値をresultに設定する
    int result = 0;

    char str[64];

    switch (sc_id) {
        case SYSCALL_ID_EXAMPLE:
            hsprintf(str, "%X %X %X %X %X %X", sc_id, param1, param2, param3, param4, param5);
            bg_draw_text(20, 520, str, COL_WHITE);
            result = 0x1234abcd;
            break;

        case SYSCALL_ID_BG_DRAW_RECT:
            bg_draw_rect(param1, param2, param3, param4, param5);
            break;
            
        case SYSCALL_ID_BG_DRAW_TEXT:
            bg_draw_text(param1, param2, (char *)param3, param4);
            break;

        case SYSCALL_ID_GET_CURRENT_TIME:
            result = get_current_time();
            break;

        case SYSCALL_ID_SLEEP: {
            int milliseconds = param1;
            task_sleep(milliseconds);
            break;
        }

        case SYSCALL_ID_GET_MOUSE_POS: {
            int x, y;
            get_mouse_pos(&x, &y);
            // 座標がshortの最大値（32767）を超えることは無いので、
            // intの上位16bitと下位16bitにそれぞれ x, y の座標を設定する
            result = (x << 16) | y;
            break;
        }

        case SYSCALL_ID_WIN_CREATE: {
            RECT *rect = (RECT *) param1;
            result = (int) win_create(rect->x, rect->y, rect->w, rect->h);
            break;
        }

        case SYSCALL_ID_WIN_DRAW_LINE: {
            WINDOW *win = (WINDOW *) param1;
            VECTOR *src = (VECTOR *) param2;
            VECTOR *dst = (VECTOR *) param3;
            byte color = (byte) param4;
            win_draw_line(win, src->x, src->y, dst->x, dst->y, color);
            break;
        }

        case SYSCALL_ID_WIN_DRAW_RECT: {
            WINDOW *win = (WINDOW *) param1;
            RECT *rect = (RECT *) param2;
            byte color = (byte) param3;
            win_draw_rect(win, rect->x, rect->y, rect->w, rect->h, color);
            break;
        }

        case SYSCALL_ID_WIN_DRAW_BYTES: {
            WINDOW *win = (WINDOW *) param1;
            RECT *rect = (RECT *) param2;
            byte *data = (byte *) param3;
            win_draw_bytes(win, rect->x, rect->y, rect->w, rect->h, data);
            break;
        }

        case SYSCALL_ID_WIN_DRAW_TEXT: {
            WINDOW *win = (WINDOW *) param1;
            int x = param2;
            int y = param3;
            char *text = (char *) param4;
            byte color = (byte) param5;
            win_draw_text(win, x, y, text, color);
            break;
        }
    }

    // 戻り値をスタックに入れる
    int* p_result = &param5 + 1;
    *p_result = result;

    return;
}
