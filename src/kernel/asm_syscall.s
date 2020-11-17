.intel_syntax noprefix

////////////////////////////////////////////////////////////////

// 各種システムコールを以下に定義する
// システムコールは、"_sc_xxxxx" という関数名にする。
.global _sc_putchar
_sc_putchar:
    mov     edx, 1
    mov     al, [esp + 4]
    int     0x40
    ret
