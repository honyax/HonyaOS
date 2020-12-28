#include "define.h"

static int timer_count;

static int display_timer_count;
static char disp_timer[10];

void init_timer() {
    timer_count = 0;
    display_timer_count = 0;
}

void inthandler20(int *esp)
{
    _out8(PIC0_OCW2, 0x60);
    timer_count++;

    task_switch();
}

bool update_timer()
{
    if (display_timer_count == timer_count) {
        return FALSE;
    }

    display_timer_count = timer_count;
#if 0
    hsprintf(disp_timer, "%d", timer_count);
    bg_draw_rect(16, 500, 80, 16, COL_DARKBLUE);
    bg_draw_text(16, 500, disp_timer, COL_WHITE);
#else
    char time[9];
    // hour, minute, second
    int hhmmss = get_current_time();
    int hour = (hhmmss & 0x00FF0000) >> 16;
    int minute = (hhmmss & 0x0000FF00) >> 8;
    int second = hhmmss & 0x000000FF;
    hsprintf(time, "%X:%X:%X", hour, minute, second);
    bg_draw_rect(16, 500, 72, 16, COL_DARKBLUE);
    bg_draw_text(16, 500, time, COL_GREEN);
#endif

    return TRUE;
}

// 16進数で現在時刻を取得
// 0x00HHMMSS（最大値は 0x00235959）
int get_current_time()
{
    _out8(0x70, 0x04);
    int hour = _in8(0x71) & 0x3F;
    _out8(0x70, 0x02);
    int minute = _in8(0x71) & 0x7F;
    _out8(0x70, 0x00);
    int second = _in8(0x71) & 0x7F;
    return (hour << 16) | (minute << 8)  | second;
}

// TODO: 各タスクの動作中状態を表示するため、タイマカウンタを表示してみる
static int display_timer_task_count[3];
void update_timer_task(int task_index, int y)
{
    if (display_timer_task_count[task_index] == timer_count)
        return;

    display_timer_task_count[task_index] = timer_count;
    hsprintf(disp_timer, "%d", timer_count);
    bg_draw_rect(80, y, 80, 16, COL_DARKBLUE);
    bg_draw_text(80, y, disp_timer, COL_CYAN);
}
