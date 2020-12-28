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
            
        case SYSCALL_ID_PUTCHAR:
            break;

        case SYSCALL_ID_BG_DRAW_TEXT:
            bg_draw_text(param1, param2, (char *)param3, param4);
            break;
    }

    // 戻り値をスタックに入れる
    int* p_result = &param5 + 1;
    *p_result = result;

    return;
}
