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
