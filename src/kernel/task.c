#include "define.h"

#define TASK_IOMAP      0x40000000
#define TASK_EFLAGS     0x00000202      // IF = 1;
#define TASK_MAX_NUM    64              // タスクは最大64
#define TASK_INDEX_ROOT 0

// Task State Segment
// Index:0 はRoot Task
static TSS task[TASK_MAX_NUM];
static int current_task_num;

// 実行中のタスクのIndex（0:root、1～:task）
static int running_task_index;

void task_b_main();
void task_c_main();

void init_task()
{
    // 初期のrootタスクを生成
    task[TASK_INDEX_ROOT].cr3 = KERNEL_PAGE_DIR;
    task[TASK_INDEX_ROOT].ldtr = GDT_IDX_LDT * 8;
    task[TASK_INDEX_ROOT].iomap = TASK_IOMAP;
    create_root_task_descriptor(&task[TASK_INDEX_ROOT]);
    _load_tr(4 * 8);
    current_task_num = 1;

    add_task(&task_b_main);
    add_task(&task_c_main);

    running_task_index = 0;
}

void add_task(void *addr)
{
    // ページング領域（ページディレクトリ 4KB、ページテーブル 4KB）
    int page_addr = (int) hmalloc(8 * 1024);
    init_task_paging((uint *)page_addr, (uint)addr);

    int index = current_task_num;
    task[index];
    task[index].cr3 = page_addr;
    task[index].ldtr = GDT_IDX_LDT * 8;
    task[index].iomap = TASK_IOMAP;
    create_task_descriptor(&task[index], index);

    // スタック領域の確保（前半32KBは権限0、後半32KBは権限3の時に使用する）
    int task_esp = ((int)hmalloc(64 * 1024)) + 64 * 1024;
    // 権限0で動作するときのss, espを設定
    task[index].ss0 = GDT_IDX_DATA * 8;
    task[index].esp0 = task_esp - 32 * 1024;
    task[index].eip = (int) addr;
    task[index].eflags = TASK_EFLAGS;
    task[index].eax = 0;
    task[index].ecx = 0;
    task[index].edx = 0;
    task[index].ebx = 0;
    task[index].esp = task_esp;
    task[index].ebp = 0;
    task[index].esi = 0;
    task[index].edi = 0;
    int task_cs = (LDT_IDX_ROOT_CS + index * 2) * 8 | 4 | 3;
    int task_ds = (LDT_IDX_ROOT_DS + index * 2) * 8 | 4 | 3;
    task[index].es = task_ds;
    task[index].cs = task_cs;
    task[index].ss = task_ds;
    task[index].ds = task_ds;
    task[index].fs = task_ds;
    task[index].gs = task_ds;

    current_task_num++;
}

void task_switch() {
    // task_indexは、現在動作中のタスクのIndexを繰り返し
    running_task_index = (running_task_index + 1) % current_task_num;
    _farjmp(0, (running_task_index + GDT_IDX_ROOT_TSS) * 8);
}

void update_timer_task(int task_index, int y);

void task_b_main()
{
    bg_draw_text(80, 540, "This is task_b_main!", COL_CYAN);
    _sc_putchar('a');

    for (;;) {
        update_timer_task(1, 560);
    }
}

void task_c_main()
{
    bg_draw_text(80, 600, "This is task_c_main!", COL_CYAN);

    for (;;) {
        update_timer_task(2, 620);
    }
}

void task_d_main()
{
    bg_draw_text(80, 660, "This is task_d_main!", COL_CYAN);

    for (;;) {}
}
