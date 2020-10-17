.global _load_idt
.global _load_gdt
.extern idtr
.extern gdtr

_load_idt:
    lidt    idtr
    ret

_load_gdt:
    lgdt    gdtr
    ret




.global _asm_sample
_asm_sample:
    movl    $1000, %eax
    ret
