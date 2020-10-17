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
