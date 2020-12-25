.intel_syntax noprefix

////////////////////////////////////////////////////////////////

// 各種システムコールを以下に定義する
// システムコールは、"_sc_xxxxx" という関数名にする。

// サンプル用のシステムコール
// int _sc_example(int param1, int param2, int param3, int param4, int param5)
.global _sc_example
_sc_example:
    push    ebp
    mov     ebp, esp
    push    edi
    push    esi
    push    ebx
    push    edx
    push    ecx

    // 各種レジスタにパラメータ設定（edi はシステムコール番号）
    // int edi, int esi, int ebx, int edx, int ecx, int eax
    // システムコール番号:0xffff
    mov     edi, 0xffff
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

.global _sc_putchar
_sc_putchar:
    mov     edi, 1
    mov     al, [esp + 4]
    int     0x40
    ret
