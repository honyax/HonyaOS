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

void _load_idt();

void setup_gate_descriptor(int num, int base, unsigned short selector, unsigned char flags)
{
    idt[num].base_lo = (unsigned short)(base & 0x0000FFFF);
    idt[num].selector = selector;
    idt[num].reserved = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (unsigned short)((base & 0xFFFF0000) >> 16);
}

void setup_interrupt_gate(int num, void *handler)
{
    setup_gate_descriptor(num, (int)handler, IDT_INT_SELECTOR,
            IDT_FLAGS_SEGMENT_PRESENT | IDT_FLAGS_DPL_0 | IDT_FLAGS_INTGATE);
}

void init_descriptor()
{
    idtr.size = NUM_IDT * sizeof(GATE_DESCRIPTOR);
    idtr.base = idt;

    _load_idt();
}
