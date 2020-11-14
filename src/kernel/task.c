#include "define.h"

typedef struct
{
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
} TSS;

TSS task_a;
TSS task_b;

#define LDT_NUM 5
#define LDT_GDT_INDEX 3
unsigned long long ldt[LDT_NUM];

void setup_segment_descriptor(int index, int limit, int base, unsigned short flags);

void task_b_main();

void init_task()
{
    // TODO: ひとまずLDTの中身をハードコーディング
    ldt[0] = 0x0000000000000000;    // zero
    ldt[1] = 0x00CF9A000000FFFF;    // task a cs
    ldt[2] = 0x00CF92000000FFFF;    // task a ds
    ldt[3] = 0x00CF9A000000FFFF;    // task b cs
    ldt[4] = 0x00CF92000000FFFF;    // task b ds

    setup_segment_descriptor(LDT_GDT_INDEX, LDT_NUM * 8 - 1, (unsigned int)ldt, 0x0082);

    task_a.cr3 = KERNEL_PAGE_DIR;
    task_a.ldtr = LDT_GDT_INDEX * 8;
    task_a.iomap = 0x40000000;
    task_b.cr3 = KERNEL_PAGE_DIR;
    task_b.ldtr = LDT_GDT_INDEX * 8;
    task_b.iomap = 0x40000000;

    setup_segment_descriptor(4, 103, (unsigned int) &task_a, 0x0089);
    setup_segment_descriptor(5, 103, (unsigned int) &task_b, 0x0089);
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
    task_b.es = 4 * 8 | 4;
    task_b.cs = 3 * 8 | 4;
    task_b.ss = 4 * 8 | 4;
    task_b.ds = 4 * 8 | 4;
    task_b.fs = 4 * 8 | 4;
    task_b.gs = 4 * 8 | 4;

    _load_gdt();
}

int current_task = 4;

void task_switch() {
    switch (current_task) {
        case 4:
            current_task = 5;
            break;
        case 5:
            current_task = 4;
            break;
    }
    _farjmp(0, current_task * 8);
}

void task_b_main()
{
    draw_text(80, 540, "This is task_b_main!", COL_CYAN);

    for (;;) {
        _hlt();
    }
}
