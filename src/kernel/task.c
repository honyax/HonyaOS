#include "define.h"

typedef struct
{
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
} TSS;

static TSS task_a;
static TSS task_b;
static TSS task_c;

#define LDT_NUM 10
#define LDT_GDT_INDEX 3
static unsigned long long ldt[LDT_NUM];

void setup_segment_descriptor(int index, int limit, int base, unsigned short flags);

static int task_index;

void task_b_main();
void task_c_main();

void init_task()
{
    // TODO: ひとまずLDTの中身をハードコーディング
    ldt[0] = 0x0000000000000000;    // zero
    ldt[1] = 0x00CF9A000000FFFF;    // task a cs
    ldt[2] = 0x00CF92000000FFFF;    // task a ds
    ldt[3] = 0x00CF9A000000FFFF;    // task b cs
    ldt[4] = 0x00CF92000000FFFF;    // task b ds
    ldt[5] = 0x00CF9A000000FFFF;    // task c cs
    ldt[6] = 0x00CF92000000FFFF;    // task c ds

    setup_segment_descriptor(LDT_GDT_INDEX, LDT_NUM * 8 - 1, (unsigned int)ldt, 0x0082);

    task_a.cr3 = KERNEL_PAGE_DIR;
    task_a.ldtr = LDT_GDT_INDEX * 8;
    task_a.iomap = 0x40000000;
    task_b.cr3 = KERNEL_PAGE_DIR;
    task_b.ldtr = LDT_GDT_INDEX * 8;
    task_b.iomap = 0x40000000;
    task_c.cr3 = KERNEL_PAGE_DIR;
    task_c.ldtr = LDT_GDT_INDEX * 8;
    task_c.iomap = 0x40000000;

    setup_segment_descriptor(4, 103, (unsigned int) &task_a, 0x0089);
    setup_segment_descriptor(5, 103, (unsigned int) &task_b, 0x0089);
    setup_segment_descriptor(6, 103, (unsigned int) &task_c, 0x0089);
    _load_tr(4 * 8);

    int task_b_esp = ((int)hmalloc(64 * 1024)) + 64 * 1024;
    task_b.eip = (int) &task_b_main;
    task_b.eflags = 0x00000202; /* IF = 1; */
    task_b.eax = 0;
    task_b.ecx = 0;
    task_b.edx = 0;
    task_b.ebx = 0;
    task_b.esp = task_b_esp;
    task_b.ebp = 0;
    task_b.esi = 0;
    task_b.edi = 0;
    // TODO: ひとまずCS/DSをハードコーディング
    int task_b_cs = 3 * 8 | 4;
    int task_b_ds = 4 * 8 | 4;
    task_b.es = task_b_ds;
    task_b.cs = task_b_cs;
    task_b.ss = task_b_ds;
    task_b.ds = task_b_ds;
    task_b.fs = task_b_ds;
    task_b.gs = task_b_ds;

    int task_c_esp = ((int)hmalloc(64 * 1024)) + 64 * 1024;
    task_c.eip = (int) &task_c_main;
    task_c.eflags = 0x00000202; /* IF = 1; */
    task_c.eax = 0;
    task_c.ecx = 0;
    task_c.edx = 0;
    task_c.ebx = 0;
    task_c.esp = task_c_esp;
    task_c.ebp = 0;
    task_c.esi = 0;
    task_c.edi = 0;
    int task_c_cs = 5 * 8 | 4;
    int task_c_ds = 6 * 8 | 4;
    task_c.es = task_c_ds;
    task_c.cs = task_c_cs;
    task_c.ss = task_c_ds;
    task_c.ds = task_c_ds;
    task_c.fs = task_c_ds;
    task_c.gs = task_c_ds;

    _load_gdt();

    task_index = 0;
}

void task_switch() {
    // task_indexは0～2を繰り返し
    task_index = (task_index + 1) % 3;
    _farjmp(0, (task_index + 4) * 8);
}

void task_b_main()
{
    draw_text(80, 540, "This is task_b_main!", COL_CYAN);

    for (;;) {
        _hlt();
    }
}

void task_c_main()
{
    draw_text(80, 600, "This is task_c_main!", COL_CYAN);

    for (;;) {
        _hlt();
    }
}
