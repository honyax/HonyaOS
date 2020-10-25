// GDT/IDT
.global _load_idt
.global _load_gdt
.extern idtr
.extern gdtr

// void _load_idt()
_load_idt:
    lidt    idtr
    ret

// void _load_gdt()
_load_gdt:
    lgdt    gdtr
    ret

// STI/CLI
.global _sti
.global _cli

// void _sti()
_sti:
    sti
    ret

// void _cli()
_cli:
    cli
    ret

// HLT
.global _hlt
.global _stihlt

// void _hlt()
_hlt:
    hlt

// void _stihlt()
_stihlt:
    sti
    hlt

////////////////////////////////////////////////////////////////

// IN/OUT
.global _in8
.global _in16
.global _in32
.global _out8
.global _out16
.global _out32

// int _in8(int port)
_in8:
    mov     0x4(%esp), %edx
    mov     $0, %eax
    in      %dx, %al
    ret

// int _in16(int port)
_in16:
    mov     0x4(%esp), %edx
    mov     $0, %eax
    in      %dx, %ax
    ret

// int _in32(int port)
_in32:
    mov     0x4(%esp), %edx
    mov     $0, %eax
    in      %dx, %eax
    ret

// void _out8(int port, int data)
_out8:
    mov     0x4(%esp), %edx
    mov     0x8(%esp), %al
    out     %al, %dx
    ret

// void _out16(int port, int data)
_out16:
    mov     0x4(%esp), %edx
    mov     0x8(%esp), %eax
    out     %ax, %dx
    ret

// void _out32(int port, int data)
_out32:
    mov     0x4(%esp), %edx
    mov     0x8(%esp), %eax
    out     %eax, %dx
    ret

////////////////////////////////////////////////////////////////

// 割り込みハンドラ
.global _asm_inthandler_default
.global _asm_inthandler21
.extern inthandler_default
.extern inthandler21

// 割り込みマクロ
.macro _asm_inthandler  c_inthandler
    push    %es
    push    %ds
    pushal
    mov     %esp, %eax
    push    %eax
    mov     %ss, %ax
    mov     %ax, %ds
    mov     %ax, %es
    call    \c_inthandler
    pop     %eax
    popal
    pop     %ds
    pop     %es
    iret
.endm

// void _asm_inthandler_default
_asm_inthandler_default:
    _asm_inthandler inthandler_default

// void _asm_inthandler21
_asm_inthandler21:
    _asm_inthandler inthandler21
