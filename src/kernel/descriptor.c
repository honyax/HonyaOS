#include "define.h"

// アライメントを2byteにしないとIDTR, GDTR構造体にパディングが入ってしまう
#pragma pack(2)

// Gate Descriptorフラグ定義値
#define IDT_FLAGS_SEGMENT_PRESENT       0x80
#define IDT_FLAGS_DPL_0                 0x00
#define IDT_FLAGS_DPL_3                 0x60
#define IDT_FLAGS_INTGATE               0x0E
#define IDT_FLAGS_TRAPGATE              0x0F

// セグメントセレクタ（カーネル用のコードディスクリプタ8を設定）
#define IDT_INT_SELECTOR                0x08

// IDTは256個
#define     NUM_IDT     256
// ゲートディスクリプタ
typedef struct
{
    unsigned short  base_lo;
    unsigned short  selector;
    unsigned char   reserved;
    unsigned char   flags;
    unsigned short  base_hi;
} GATE_DESCRIPTOR;
static GATE_DESCRIPTOR idt[NUM_IDT];

// IDTRを定義（asm.sから参照するためstaticにしない）
typedef struct 
{
    unsigned short      size;
    GATE_DESCRIPTOR*    base;
} IDTR;
IDTR idtr;

void setup_gate_descriptor(int index, int base, unsigned short selector, unsigned char flags)
{
    idt[index].base_lo = (unsigned short)(base & 0x0000FFFF);
    idt[index].selector = selector;
    idt[index].reserved = 0;
    idt[index].flags = flags;
    idt[index].base_hi = (unsigned short)((base & 0xFFFF0000) >> 16);
}

void setup_interrupt_gate(int index, void *handler)
{
    setup_gate_descriptor(index, (int)handler, IDT_INT_SELECTOR,
            IDT_FLAGS_SEGMENT_PRESENT | IDT_FLAGS_DPL_0 | IDT_FLAGS_INTGATE);
}

void setup_syscall(int index, void *handler)
{
    setup_gate_descriptor(index, (int)handler, IDT_INT_SELECTOR,
            IDT_FLAGS_SEGMENT_PRESENT | IDT_FLAGS_DPL_3 | IDT_FLAGS_TRAPGATE);
}

// GDTは十分に用意しておく
#define     NUM_GDT     1024
// セグメントディスクリプタ
typedef struct
{
    unsigned short  limit_lo;
    unsigned short  base_lo;
    unsigned char   base_mid;
    unsigned char   flags;
    unsigned char   limit_hi_flags;
    unsigned char   base_hi;
} SEGMENT_DESCRIPTOR;
static SEGMENT_DESCRIPTOR gdt[NUM_GDT];

// GDTRを定義（asm.sから参照するためstaticにしない）
typedef struct
{
    unsigned short      size;
    SEGMENT_DESCRIPTOR* base;
} GDTR;
GDTR gdtr;

#define GDT_FLAGS_LIMIT_G   0x8000
#define GDT_FLAGS_SEGM_32   0x4000
#define GDT_FLAGS_PRESENCE  0x0080
#define GDT_FLAGS_DPL_0     0x0000
#define GDT_FLAGS_DPL_3     0x0060
#define GDT_FLAGS_TYPE_SYS  0x0000
#define GDT_FLAGS_TYPE_CODE 0x009A
#define GDT_FLAGS_TYPE_DATA 0x0092
#define GDT_FLAGS_TYPE_LDT  0x0082
#define GDT_FLAGS_TYPE_TSS  0x0089
#define GDT_NULL_LIMIT      0x00000
#define GDT_NULL_BASE       0x00000000
#define GDT_NULL_FLAGS      0x0000
#define GDT_CODE_LIMIT      0xFFFFF
#define GDT_CODE_BASE       0x00000000
#define GDT_CODE_FLAGS      GDT_FLAGS_LIMIT_G | GDT_FLAGS_SEGM_32 | GDT_FLAGS_DPL_0 | GDT_FLAGS_TYPE_CODE
#define GDT_DATA_LIMIT      0xFFFFF
#define GDT_DATA_BASE       0x00000000
#define GDT_DATA_FLAGS      GDT_FLAGS_LIMIT_G | GDT_FLAGS_SEGM_32 | GDT_FLAGS_DPL_0 | GDT_FLAGS_TYPE_DATA
#define GDT_LDT_FLAGS       GDT_FLAGS_TYPE_LDT
#define GDT_TSS_LIMIT       103         // 103 = sizeof(TSS) - 1

// LDTはひとまず128用意する
#define     NUM_LDT     128
static SEGMENT_DESCRIPTOR ldt[NUM_LDT];

#define LDT_NULL_LIMIT      0x00000
#define LDT_NULL_BASE       0x00000000
#define LDT_NULL_FLAGS      0x0000
#define LDT_TASK_CODE_BASE  0x00000000
#define LDT_TASK_CODE_LIMIT 0xFFFFF
#define LDT_TASK_DATA_BASE  0x00000000
#define LDT_TASK_DATA_LIMIT 0xFFFFF
#define LDT_ROOT_CODE_FLAGS GDT_FLAGS_LIMIT_G | GDT_FLAGS_SEGM_32 | GDT_FLAGS_DPL_0 | GDT_FLAGS_TYPE_CODE
#define LDT_ROOT_DATA_FLAGS GDT_FLAGS_LIMIT_G | GDT_FLAGS_SEGM_32 | GDT_FLAGS_DPL_0 | GDT_FLAGS_TYPE_DATA
#define LDT_TASK_CODE_FLAGS GDT_FLAGS_LIMIT_G | GDT_FLAGS_SEGM_32 | GDT_FLAGS_DPL_3 | GDT_FLAGS_TYPE_CODE
#define LDT_TASK_DATA_FLAGS GDT_FLAGS_LIMIT_G | GDT_FLAGS_SEGM_32 | GDT_FLAGS_DPL_3 | GDT_FLAGS_TYPE_DATA

void setup_segment_descriptor(int index, int limit, int base, unsigned short flags)
{
    gdt[index].limit_lo = (unsigned short)(limit & 0x0000FFFF);
    gdt[index].base_lo = (unsigned short)(base & 0x0000FFFF);
    gdt[index].base_mid = (unsigned char)((base & 0x00FF0000) >> 16);
    gdt[index].flags = (unsigned char)(flags & 0x00FF);
    gdt[index].limit_hi_flags = (unsigned char)((limit & 0x00F0000) >> 16 | (flags & 0xF000) >> 8);
    gdt[index].base_hi = (unsigned char)((base & 0xFF000000) >> 24);
}

void setup_local_segment_descriptor(int index, int limit, int base, unsigned short flags)
{
    ldt[index].limit_lo = (unsigned short)(limit & 0x0000FFFF);
    ldt[index].base_lo = (unsigned short)(base & 0x0000FFFF);
    ldt[index].base_mid = (unsigned char)((base & 0x00FF0000) >> 16);
    ldt[index].flags = (unsigned char)(flags & 0x00FF);
    ldt[index].limit_hi_flags = (unsigned char)((limit & 0x00F0000) >> 16 | (flags & 0xF000) >> 8);
    ldt[index].base_hi = (unsigned char)((base & 0xFF000000) >> 24);
}

void init_descriptor()
{
    // GDTの初期化
    for (int i = 0; i < NUM_GDT; i++) {
        setup_segment_descriptor(i, GDT_NULL_LIMIT, GDT_NULL_BASE, GDT_NULL_FLAGS);
    }
    setup_segment_descriptor(GDT_IDX_CODE, GDT_CODE_LIMIT, GDT_CODE_BASE, GDT_CODE_FLAGS);
    setup_segment_descriptor(GDT_IDX_DATA, GDT_DATA_LIMIT, GDT_DATA_BASE, GDT_DATA_FLAGS);
    setup_segment_descriptor(GDT_IDX_LDT, sizeof(ldt) - 1, (unsigned int)ldt, GDT_LDT_FLAGS);
    gdtr.size = NUM_GDT * sizeof(SEGMENT_DESCRIPTOR);
    gdtr.base = gdt;
    _load_gdt();

    // LDTの初期化
    setup_local_segment_descriptor(LDT_IDX_NULL, LDT_NULL_LIMIT, LDT_NULL_BASE, LDT_NULL_FLAGS);

    // 割り込みの初期化
    for (int i = 0; i < NUM_IDT; i++) {
        setup_interrupt_gate(i, _asm_inthandler_default);
    }
    idtr.size = NUM_IDT * sizeof(GATE_DESCRIPTOR);
    idtr.base = idt;
    _load_idt();

    setup_interrupt_gate(0x00, _asm_inthandler00);      // 0除算
    setup_interrupt_gate(0x0e, _asm_inthandler0e);      // ページフォールト
    setup_interrupt_gate(0x20, _asm_inthandler20);      // タイマ割り込み
    setup_interrupt_gate(0x21, _asm_inthandler21);      // キーボード割り込み
    setup_interrupt_gate(0x2c, _asm_inthandler2c);      // マウス割り込み
    setup_syscall(0x40, _asm_syscall);
}

void create_root_task(TSS *task_root)
{
    // 初期のタスク（rootプロセスに相当）に関するGDT, LDTの設定
    setup_local_segment_descriptor(LDT_IDX_ROOT_CS, LDT_TASK_CODE_LIMIT, LDT_TASK_CODE_BASE, LDT_ROOT_CODE_FLAGS);
    setup_local_segment_descriptor(LDT_IDX_ROOT_DS, LDT_TASK_DATA_LIMIT, LDT_TASK_DATA_BASE, LDT_ROOT_DATA_FLAGS);
    setup_segment_descriptor(GDT_IDX_ROOT_TSS, GDT_TSS_LIMIT, (unsigned int) task_root, GDT_FLAGS_TYPE_TSS);
    _load_tr(4 * 8);
}

void create_task(TSS *task, int task_index)
{
    // タスクごとにCS, DSをLDTに登録
    setup_local_segment_descriptor(LDT_IDX_ROOT_CS + task_index * 2, LDT_TASK_CODE_LIMIT, LDT_TASK_CODE_BASE, LDT_TASK_CODE_FLAGS);
    setup_local_segment_descriptor(LDT_IDX_ROOT_DS + task_index * 2, LDT_TASK_DATA_LIMIT, LDT_TASK_DATA_BASE, LDT_TASK_DATA_FLAGS);
    setup_segment_descriptor(GDT_IDX_ROOT_TSS + task_index, GDT_TSS_LIMIT, (unsigned int) task, GDT_FLAGS_TYPE_TSS);
}
