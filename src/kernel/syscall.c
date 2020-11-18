#include "define.h"

// 各システムコールの処理
void syscall(int edi, int esi, int ebx, int edx, int ecx, int eax)
{
    // アプリケーションに渡す戻り値をresultに設定する
    int result = 0;

    char str[64];

    switch (edi) {
        case 0xffff:
            hsprintf(str, "%X %X %X %X %X %X", edi, esi, ebx, edx, ecx, eax);
            draw_text(20, 520, str, COL_WHITE);
            result = 0x1234abcd;
            break;
            
        case 1:
            break;
    }

    // 戻り値をスタックに入れる
    int* p_result = &eax + 1;
    *p_result = result;

    return;
}
