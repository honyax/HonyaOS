.global _load_idt
.extern idtr

_load_idt:
    lidt    idtr
    ret
