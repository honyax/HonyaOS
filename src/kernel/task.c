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

void setup_segment_descriptor(int index, int limit, int base, unsigned short flags);

void task_b_main();

void init_task()
{
    task_a.cr3 = KERNEL_PAGE_DIR;
    task_a.ldtr = 0;
    task_a.iomap = 0x40000000;
    task_b.cr3 = KERNEL_PAGE_DIR;
    task_b.ldtr = 0;
    task_b.iomap = 0x40000000;

    setup_segment_descriptor(3, 103, (unsigned int) &task_a, 0x0089);
    setup_segment_descriptor(4, 103, (unsigned int) &task_b, 0x0089);
    _load_tr(3 * 8);

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
    task_b.es = 2 * 8;
    task_b.cs = 1 * 8;
    task_b.ss = 2 * 8;
    task_b.ds = 2 * 8;
    task_b.fs = 2 * 8;
    task_b.gs = 2 * 8;
}

void task_switch() {
    _farjmp(0, 4 * 8);
}

void task_b_main()
{
    for (;;) {
        _hlt();
    }
}
