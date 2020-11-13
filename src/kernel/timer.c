#include "define.h"

int timer_count = 0;

void inthandler20(int *esp)
{
    _out8(PIC0_OCW2, 0x60);
    timer_count++;

    task_switch();
}

int display_timer_count = 0;
char disp_timer[10];

void update_timer()
{
    if (display_timer_count != timer_count) {
        display_timer_count = timer_count;
        hsprintf(disp_timer, "%d", display_timer_count);
        draw_rect(16, 500, 80, 16, COL_BLACK);
        draw_text(16, 500, disp_timer, COL_WHITE);
    }
}
