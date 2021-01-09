.intel_syntax noprefix

////////////////////////////////////////////////////////////////

// システムコール用マクロ

.macro _asm_syscall_5 c_syscall_id
    push    ebp
    mov     ebp, esp
    push    edi
    push    esi
    push    ebx
    push    edx
    push    ecx

    // 各種レジスタにパラメータ設定（edi はシステムコール番号）
    // int edi, int esi, int ebx, int edx, int ecx, int eax
    mov     edi, \c_syscall_id
    mov     esi, [ebp+ 8]
    mov     ebx, [ebp+12]
    mov     edx, [ebp+16]
    mov     ecx, [ebp+20]
    mov     eax, [ebp+24]
    int     0x40

    pop     ecx
    pop     edx
    pop     ebx
    pop     esi
    pop     edi
    pop     ebp
    ret
.endm

.macro _asm_syscall_4 c_syscall_id
    push    ebp
    mov     ebp, esp
    push    edi
    push    esi
    push    ebx
    push    edx
    push    ecx

    // 各種レジスタにパラメータ設定（edi はシステムコール番号）
    // int edi, int esi, int ebx, int edx, int ecx
    mov     edi, \c_syscall_id
    mov     esi, [ebp+ 8]
    mov     ebx, [ebp+12]
    mov     edx, [ebp+16]
    mov     ecx, [ebp+20]
    int     0x40

    pop     ecx
    pop     edx
    pop     ebx
    pop     esi
    pop     edi
    pop     ebp
    ret
.endm

.macro _asm_syscall_3 c_syscall_id
    push    ebp
    mov     ebp, esp
    push    edi
    push    esi
    push    ebx
    push    edx

    // 各種レジスタにパラメータ設定（edi はシステムコール番号）
    // int edi, int esi, int ebx, int edx
    mov     edi, \c_syscall_id
    mov     esi, [ebp+ 8]
    mov     ebx, [ebp+12]
    mov     edx, [ebp+16]
    int     0x40

    pop     edx
    pop     ebx
    pop     esi
    pop     edi
    pop     ebp
    ret
.endm

.macro _asm_syscall_2 c_syscall_id
    push    ebp
    mov     ebp, esp
    push    edi
    push    esi
    push    ebx

    // 各種レジスタにパラメータ設定（edi はシステムコール番号）
    // int edi, int esi, int ebx
    mov     edi, \c_syscall_id
    mov     esi, [ebp+ 8]
    mov     ebx, [ebp+12]
    int     0x40

    pop     ebx
    pop     esi
    pop     edi
    pop     ebp
    ret
.endm

.macro _asm_syscall_1 c_syscall_id
    push    ebp
    mov     ebp, esp
    push    edi
    push    esi

    // 各種レジスタにパラメータ設定（edi はシステムコール番号）
    // int edi, int esi
    mov     edi, \c_syscall_id
    mov     esi, [ebp+ 8]
    int     0x40

    pop     esi
    pop     edi
    pop     ebp
    ret
.endm

.macro _asm_syscall_0 c_syscall_id
    push    ebp
    mov     ebp, esp
    push    edi

    // 各種レジスタにパラメータ設定（edi はシステムコール番号）
    // int edi
    mov     edi, \c_syscall_id
    int     0x40

    pop     edi
    pop     ebp
    ret
.endm

// 各種システムコールを以下に定義する
// システムコールは、"_sc_xxxxx" という関数名にする。

// サンプル用のシステムコール
// int _sc_example(int param1, int param2, int param3, int param4, int param5)
.global _sc_example
_sc_example:
    _asm_syscall_5 0xFFFF

.global _sc_bg_draw_text
_sc_bg_draw_text:
    _asm_syscall_4 0x100

.global _sc_get_current_time
_sc_get_current_time:
    _asm_syscall_0 0x200

.global _sc_sleep
_sc_sleep:
    _asm_syscall_1 0x201

.global _sc_get_mouse_pos
_sc_get_mouse_pos:
    _asm_syscall_0 0x202

.global _sc_win_create
_sc_win_create:
    _asm_syscall_1 0x300

.global _sc_win_draw_line
_sc_win_draw_line:
    _asm_syscall_4 0x301

.global _sc_win_draw_rect
_sc_win_draw_rect:
    _asm_syscall_3 0x302

.global _sc_win_draw_bytes
_sc_win_draw_bytes:
    _asm_syscall_3 0x303

.global _sc_win_draw_text
_sc_win_draw_text:
    _asm_syscall_5 0x304
