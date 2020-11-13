#include "define.h"

int timer_count = 0;

void inthandler20(int *esp)
{
    _out8(PIC0_OCW2, 0x60);
    timer_count++;
}

int display_timer_count = 0;
char disp_timer[10];
int current_task = 3;

void update_timer()
{
    if (display_timer_count != timer_count) {
        display_timer_count = timer_count;
        hsprintf(disp_timer, "%d", display_timer_count);
        draw_rect(16, 500, 80, 16, COL_BLACK);
        draw_text(16, 500, disp_timer, COL_WHITE);
    }

    switch (current_task) {
        case 3:
            current_task = 4;
            break;
        case 4:
            current_task = 3;
            break;
    }
    task_switch(current_task);
}
