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
GATE_DESCRIPTOR idt[NUM_IDT];

// IDTRを定義
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
SEGMENT_DESCRIPTOR gdt[NUM_GDT];

// GDTRを定義
typedef struct
{
    unsigned short      size;
    SEGMENT_DESCRIPTOR* base;
} GDTR;
GDTR gdtr;

// Segment Descriptor定義値
#define GDT_IDX_NULL        0
#define GDT_IDX_CODE        1
#define GDT_IDX_DATA        2
#define GDT_IDX_TEMP        3
#define GDT_IDX_TASK_CODE   3
#define GDT_IDX_TASK_DATA   4
#define GDT_IDX_KTSS        5

#define GDT_FLAGS_DEFAULT   0xC082
#define GDT_FLAGS_DPL_0     0x0000
#define GDT_FLAGS_DPL_3     0x0060
#define GDT_FLAGS_TYPE_SYS  0x0000
#define GDT_FLAGS_TYPE_CODE 0x0018
#define GDT_FLAGS_TYPE_DATA 0x0010
#define GDT_NULL_LIMIT      0x00000
#define GDT_NULL_BASE       0x00000000
#define GDT_NULL_FLAGS      0x0000
#define GDT_CODE_LIMIT      0xFFFFF
#define GDT_CODE_BASE       0x00000000
#define GDT_CODE_FLAGS      GDT_FLAGS_DEFAULT | GDT_FLAGS_DPL_0 | GDT_FLAGS_TYPE_CODE
#define GDT_DATA_LIMIT      0xFFFFF
#define GDT_DATA_BASE       0x00000000
#define GDT_DATA_FLAGS      GDT_FLAGS_DEFAULT | GDT_FLAGS_DPL_0 | GDT_FLAGS_TYPE_DATA

void setup_segment_descriptor(int index, int limit, int base, unsigned short flags)
{
    gdt[index].limit_lo = (unsigned short)(limit & 0x0000FFFF);
    gdt[index].base_lo = (unsigned short)(base & 0x0000FFFF);
    gdt[index].base_mid = (unsigned char)((base & 0x00FF0000) >> 16);
    gdt[index].flags = (unsigned char)(flags & 0x00FF);
    gdt[index].limit_hi_flags = (unsigned char)((limit & 0x00F0000) >> 16 | (flags & 0xF000) >> 8);
    gdt[index].base_hi = (unsigned char)((base & 0xFF000000) >> 24);
}

void init_descriptor()
{
    for (int i = 0; i < NUM_GDT; i++) {
        setup_segment_descriptor(i, GDT_NULL_LIMIT, GDT_NULL_BASE, GDT_NULL_FLAGS);
    }
    setup_segment_descriptor(GDT_IDX_CODE, GDT_CODE_LIMIT, GDT_CODE_BASE, GDT_CODE_FLAGS);
    setup_segment_descriptor(GDT_IDX_DATA, GDT_DATA_LIMIT, GDT_DATA_BASE, GDT_DATA_FLAGS);
    gdtr.size = NUM_GDT * sizeof(SEGMENT_DESCRIPTOR);
    gdtr.base = gdt;
    _load_gdt();

    for (int i = 0; i < NUM_IDT; i++) {
        setup_interrupt_gate(i, _asm_inthandler_default);
    }
    idtr.size = NUM_IDT * sizeof(GATE_DESCRIPTOR);
    idtr.base = idt;
    _load_idt();

    setup_interrupt_gate(0x00, _asm_inthandler00);
    setup_interrupt_gate(0x20, _asm_inthandler20);
    setup_interrupt_gate(0x21, _asm_inthandler21);
    setup_interrupt_gate(0x2c, _asm_inthandler2c);
}
