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

void update_timer()
{
    if (display_timer_count == timer_count)
        return;

    display_timer_count = timer_count;
    hsprintf(disp_timer, "%d", timer_count);
    draw_rect(16, 500, 80, 16, COL_BLACK);
    draw_text(16, 500, disp_timer, COL_WHITE);
}

// TODO: 各タスクの動作中状態を表示するため、タイマカウンタを表示してみる
static int display_timer_task_count[3];
void update_timer_task(int task_index, int y)
{
    if (display_timer_task_count[task_index] == timer_count)
        return;

    display_timer_task_count[task_index] = timer_count;
    hsprintf(disp_timer, "%d", timer_count);
    draw_rect(80, y, 80, 16, COL_BLACK);
    draw_text(80, y, disp_timer, COL_CYAN);
}
