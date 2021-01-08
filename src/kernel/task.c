#include "define.h"

#define TASK_IOMAP      0x40000000
#define TASK_EFLAGS     0x00000202      // IF = 1;
#define TASK_MAX_NUM    64              // タスクは最大64
#define TASK_INDEX_ROOT 0

typedef enum {
    TASK_STATE_NONE = 1,
    TASK_STATE_WORK,
    TASK_STATE_SLEEP,
} TASK_STATE;

typedef struct {
    TASK_STATE state;
    TSS tss;
    int wake_count;
} TASK_INFORMATION;

// Task State Segment
// Index:0 はRoot Task
static TASK_INFORMATION task[TASK_MAX_NUM];

// 実行中のタスクのIndex（0:root、1～:task）
static int running_task_index;

void task_b_main();
void task_c_main();

void init_task()
{
    // タスクの状態を初期化
    for (int i = 0; i < TASK_MAX_NUM; i++) {
        task[i].state= TASK_STATE_NONE;
    }

    // 初期のrootタスクを生成
    task[TASK_INDEX_ROOT].tss.cr3 = KERNEL_PAGE_DIR;
    task[TASK_INDEX_ROOT].tss.ldtr = GDT_IDX_LDT * 8;
    task[TASK_INDEX_ROOT].tss.iomap = TASK_IOMAP;
    task[TASK_INDEX_ROOT].state = TASK_STATE_WORK;
    create_root_task_descriptor(&task[TASK_INDEX_ROOT].tss);
    _load_tr(4 * 8);

    add_task(&task_b_main);
    add_task(&task_c_main);

    running_task_index = 0;
}

void add_task(void *addr)
{
    // ページング領域（ページディレクトリ 4KB、ページテーブル 4KB）
    int page_addr = (int) hmalloc(8 * 1024);
    init_task_paging((uint *)page_addr, (uint)addr);

    int index;
    for (index = 0; index < TASK_MAX_NUM; index++) {
        if (task[index].state == TASK_STATE_NONE) {
            break;
        }
    }
    if (index == TASK_MAX_NUM) {
        // タスクの空きがない場合は無処理
        return;
    }

    task[index].tss.cr3 = page_addr;
    task[index].tss.ldtr = GDT_IDX_LDT * 8;
    task[index].tss.iomap = TASK_IOMAP;
    task[index].state = TASK_STATE_WORK;
    create_task_descriptor(&task[index].tss, index);

    // スタック領域の確保（前半32KBは権限0、後半32KBは権限3の時に使用する）
    int task_esp = ((int)hmalloc(64 * 1024)) + 64 * 1024;
    // 権限0で動作するときのss, espを設定
    task[index].tss.ss0 = GDT_IDX_DATA * 8;
    task[index].tss.esp0 = task_esp - 32 * 1024;
    task[index].tss.eip = (int) addr;
    task[index].tss.eflags = TASK_EFLAGS;
    task[index].tss.eax = 0;
    task[index].tss.ecx = 0;
    task[index].tss.edx = 0;
    task[index].tss.ebx = 0;
    task[index].tss.esp = task_esp;
    task[index].tss.ebp = 0;
    task[index].tss.esi = 0;
    task[index].tss.edi = 0;
    int task_cs = (LDT_IDX_ROOT_CS + index * 2) * 8 | 4 | 3;
    int task_ds = (LDT_IDX_ROOT_DS + index * 2) * 8 | 4 | 3;
    task[index].tss.es = task_ds;
    task[index].tss.cs = task_cs;
    task[index].tss.ss = task_ds;
    task[index].tss.ds = task_ds;
    task[index].tss.fs = task_ds;
    task[index].tss.gs = task_ds;
}

// 現在のタスクを一定時間SLEEP状態にする
void task_sleep(int milliseconds)
{
    // 指定はミリ秒だが、粒度は10msなので10で割って切り上げ
    int sleep_count = milliseconds / 10 + ((milliseconds % 10) > 0 ? 1 : 0);

    // sleepが終了するカウントを算出
    int wake_count = get_timer_count() + sleep_count;
    task[running_task_index].state = TASK_STATE_SLEEP;
    task[running_task_index].wake_count = wake_count;

    // 次のタイマ割り込みを待たずに、強制的にタスクを切り替える
    task_switch();
}

void task_switch() {
    int old_runnning_task_index = running_task_index;
    // task_indexは、現在動作中のタスクのIndexを繰り返す
    running_task_index++;
    int current_timer_count = get_timer_count();
    for (; running_task_index < TASK_MAX_NUM; running_task_index++) {
        if (task[running_task_index].state == TASK_STATE_WORK) {
            break;
        } else if (task[running_task_index].state == TASK_STATE_SLEEP) {
            // sleep状態のタスクは、wake_countを過ぎた時点で稼働させる
            if (task[running_task_index].wake_count < current_timer_count) {
                task[running_task_index].state = TASK_STATE_WORK;
                break;
            }
        }
    }

    // タスクIndexが最大値に達したらIndex:0を実行する。rootタスクがsleepになることはないので無条件で切り替えてOK.
    if (running_task_index == TASK_MAX_NUM) {
        running_task_index = 0;
    }

    // rootタスクだけの場合は切り替えられない
    if (old_runnning_task_index != running_task_index) {
        _farjmp(0, (running_task_index + GDT_IDX_ROOT_TSS) * 8);
    }
}

void update_timer_task(int task_index, int y);

void task_b_main()
{
    bg_draw_text(80, 540, "This is task_b_main!", COL_CYAN);
    _sc_putchar('a');

    for (;;) {
        _sc_sleep(1000);
        update_timer_task(1, 560);
    }
}

void task_c_main()
{
    bg_draw_text(80, 600, "This is task_c_main!", COL_CYAN);

    for (;;) {
        _sc_sleep(33);
        update_timer_task(2, 620);
    }
}

void task_d_main()
{
    bg_draw_text(80, 660, "This is task_d_main!", COL_CYAN);

    for (;;) {}
}
