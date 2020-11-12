.intel_syntax noprefix

////////////////////////////////////////////////////////////////

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
    ret

// void _stihlt()
_stihlt:
    sti
    hlt
    ret

// Task
.global _load_tr
.global _farjmp

// void _load_tr(int tr)
_load_tr:
    ltr     [esp + 0x4]
    ret

// void _farjmp(int eip, short cs)
_farjmp:
    jmp fword ptr [esp+4]
    ret

// CR0/CR3
.global _get_cr0
.global _set_cr0
.global _set_cr3

// unsigned int _get_cr0()
_get_cr0:
    mov     eax, cr0
    ret

// void _set_cr0(unsigned int value)
_set_cr0:
    mov     eax, [esp + 0x4]
    mov     cr0, eax
    jmp     $ + 2
    ret

// void _set_cr3(unsigned int value)
_set_cr3:
    mov     eax, [esp + 0x4]
    mov     cr3, eax
    ret

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
    mov     edx, [esp + 0x4]
    mov     eax, 0
    in      al, dx
    ret

// int _in16(int port)
_in16:
    mov     edx, [esp + 0x4]
    mov     eax, 0
    in      ax, dx
    ret

// int _in32(int port)
_in32:
    mov     edx, [esp + 0x4]
    mov     eax, 0
    in      eax, dx
    ret

// void _out8(int port, int data)
_out8:
    mov     edx, [esp + 0x4]
    mov     al, [esp + 0x8]
    out     dx, al
    ret

// void _out16(int port, int data)
_out16:
    mov     edx, [esp + 0x4]
    mov     eax, [esp + 0x8]
    out     dx, ax
    ret

// void _out32(int port, int data)
_out32:
    mov     edx, [esp + 0x4]
    mov     eax, [esp + 0x8]
    out     dx, eax
    ret

////////////////////////////////////////////////////////////////

// 割り込みハンドラ
.global _asm_inthandler_default
.global _asm_inthandler00
.global _asm_inthandler0e
.global _asm_inthandler20
.global _asm_inthandler21
.global _asm_inthandler2c
.extern inthandler_default
.extern inthandler00
.extern inthandler0e
.extern inthandler20
.extern inthandler21
.extern inthandler2c

// 割り込みマクロ
.macro _asm_inthandler  c_inthandler
    push    es
    push    ds
    pusha
    mov     eax, esp
    push    eax
    mov     ax, ss
    mov     ds, ax
    mov     es, ax
    call    \c_inthandler
    pop     eax
    popa
    pop     ds
    pop     es
    iret
.endm

// void _asm_inthandler_default
_asm_inthandler_default:
    _asm_inthandler inthandler_default

// void _asm_inthandler00
_asm_inthandler00:
    _asm_inthandler inthandler00

// void _asm_inthandler0e
_asm_inthandler0e:
    _asm_inthandler inthandler0e

// void _asm_inthandler20
_asm_inthandler20:
    _asm_inthandler inthandler20

// void _asm_inthandler21
_asm_inthandler21:
    _asm_inthandler inthandler21

// void _asm_inthandler2c
_asm_inthandler2c:
    _asm_inthandler inthandler2c
