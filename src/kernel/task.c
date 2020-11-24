#include "define.h"

#define TASK_IOMAP      0x40000000
#define TAKS_EFLAGS     0x00000202      // IF = 1;

// root task
static TSS task_root;

// TODO: ひとまずTSSを個別の変数で保持
// tasks
static TSS task_b;
static TSS task_c;

// 実行中のタスクのIndex（0:root、1～:task）
static int running_task_index;

void task_b_main();
void task_c_main();

void init_task()
{
    // 初期のrootタスクを生成
    task_root.cr3 = KERNEL_PAGE_DIR;
    task_root.ldtr = GDT_IDX_LDT * 8;
    task_root.iomap = TASK_IOMAP;
    create_root_task(&task_root);

    int task_index_b = 1;
    int task_index_c = 2;

    task_b.cr3 = KERNEL_PAGE_DIR;
    task_b.ldtr = GDT_IDX_LDT * 8;
    task_b.iomap = TASK_IOMAP;
    task_c.cr3 = KERNEL_PAGE_DIR;
    task_c.ldtr = GDT_IDX_LDT * 8;
    task_c.iomap = TASK_IOMAP;
    create_task(&task_b, task_index_b);
    create_task(&task_c, task_index_c);

    // スタック領域の確保（前半32KBは権限0、後半32KBは権限3の時に使用する）
    int task_b_esp = ((int)hmalloc(64 * 1024)) + 64 * 1024;
    // 権限0で動作するときのss, espを設定
    task_b.ss0 = GDT_IDX_DATA * 8;
    task_b.esp0 = task_b_esp - 32 * 1024;
    task_b.eip = (int) &task_b_main;
    task_b.eflags = TAKS_EFLAGS;
    task_b.eax = 0;
    task_b.ecx = 0;
    task_b.edx = 0;
    task_b.ebx = 0;
    task_b.esp = task_b_esp;
    task_b.ebp = 0;
    task_b.esi = 0;
    task_b.edi = 0;
    int task_b_cs = (LDT_IDX_ROOT_CS + task_index_b * 2) * 8 | 4 | 3;
    int task_b_ds = (LDT_IDX_ROOT_DS + task_index_b * 2) * 8 | 4 | 3;
    task_b.es = task_b_ds;
    task_b.cs = task_b_cs;
    task_b.ss = task_b_ds;
    task_b.ds = task_b_ds;
    task_b.fs = task_b_ds;
    task_b.gs = task_b_ds;

    int task_c_esp = ((int)hmalloc(64 * 1024)) + 64 * 1024;
    task_c.ss0 = GDT_IDX_DATA * 8;
    task_c.esp0 = task_c_esp - 32 * 1024;
    task_c.eip = (int) &task_c_main;
    task_c.eflags = TAKS_EFLAGS;
    task_c.eax = 0;
    task_c.ecx = 0;
    task_c.edx = 0;
    task_c.ebx = 0;
    task_c.esp = task_c_esp;
    task_c.ebp = 0;
    task_c.esi = 0;
    task_c.edi = 0;
    int task_c_cs = (LDT_IDX_ROOT_CS + task_index_c * 2) * 8 | 4 | 3;
    int task_c_ds = (LDT_IDX_ROOT_DS + task_index_c * 2) * 8 | 4 | 3;
    task_c.es = task_c_ds;
    task_c.cs = task_c_cs;
    task_c.ss = task_c_ds;
    task_c.ds = task_c_ds;
    task_c.fs = task_c_ds;
    task_c.gs = task_c_ds;

    running_task_index = 0;
}

void task_switch() {
    // task_indexは0～2を繰り返し
    running_task_index = (running_task_index + 1) % 3;
    _farjmp(0, (running_task_index + GDT_IDX_ROOT_TSS) * 8);
}

void update_timer_task(int task_index, int y);

void task_b_main()
{
    draw_text(80, 540, "This is task_b_main!", COL_CYAN);
    _sc_putchar('a');

    for (;;) {
        update_timer_task(1, 560);
    }
}

void task_c_main()
{
    int result = _sc_example(0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555);
    char str[32];
    hsprintf(str, "This is task_c_main! %X", result);
    draw_text(80, 600, str, COL_CYAN);

    for (;;) {
        update_timer_task(2, 620);
    }
}
