;************************************************************************
;   ブートプログラム
;
;   プログラム全体を通して、セグメントの値は0x0000とする。
;   (DS==ES==0)
;
;************************************************************************

;************************************************************************
;   マクロ
;************************************************************************
%include    "define.s"
%include    "macro.s"

        ORG         BOOT_LOAD               ; ロード先のアドレス(0x7C00)を指定

;************************************************************************
;   エントリポイント
;************************************************************************
entry:
		;---------------------------------------
		; BPB(BIOS Parameter Block)
		;---------------------------------------
        jmp     ipl                         ; iplへジャンプ
        times   90 - ($ - $$) db 0x90       ; 90の位置まで0x90(NOP)で埋める

		;---------------------------------------
		; IPL(Initial Program Loader)
		;---------------------------------------
ipl:
        cli                                 ; 割り込み禁止

        mov     ax, 0x0000
        mov     ds, ax
        mov     es, ax
        mov     ss, ax
        mov     sp, BOOT_LOAD               ; SP = 0x7C00

        sti                                 ; 割り込み許可

        mov     [boot + drive.no], dl       ; ブートドライブを保存

        cdecl   puts, .s0                   ; puts(.s0);

        ;--------------------------------
        ; 残りのセクタを全て読み込む
        ;--------------------------------
        mov     bx, BOOT_SECT - 1           ; BX = 残りのブートセクタ数
        mov     cx, BOOT_LOAD + SECT_SIZE   ; CX = 次のロードアドレス

        cdecl   read_chs, boot, bx, cx      ; AX = read_chs(.chs, bx, cx);

        cmp     ax, bx
        jz      .success                    ; if (AX == BX) goto .success

.error:
        cdecl   puts, .e0                   ; エラーメッセージ表示
        jmp     $                           ; while (true) ;

.success:
        cdecl   puts, .s1                   ; 成功メッセージ表示
        jmp     stage_2                     ; goto stage_2

        ;--------------------------------
        ; データ
        ;--------------------------------
.s0:    db      "HonyaOS Booting...", 0x0A, 0x0D, 0
.s1:    db      "Success:sector read", 0x0A, 0x0D, 0
.e0:    db      "Error:sector read", 0x0A, 0x0D, 0

;************************************************************************
;   ブートドライブに関する情報
;************************************************************************
ALIGN 2, db 0
boot:                                       ; ブートドライブに関する情報
    istruc  drive
        at  drive.no,       dw  0           ; ドライブ番号
        at  drive.cyln,     dw  0           ; C:シリンダ
        at  drive.head,     dw  0           ; H:ヘッド
        at  drive.sect,     dw  2           ; S:セクタ
    iend

;************************************************************************
;   モジュール
;************************************************************************
%include    "modules/real/puts.s"
%include    "modules/real/read_chs.s"

;************************************************************************
;   ブートフラグ（先頭512バイトの終了）
;************************************************************************
        times   510 - ($ - $$) db 0x00
        db      0x55, 0xAA

;************************************************************************
;   リアルモード時に取得した情報
;   プロテクトモードでも参照できるよう、固定の位置（0x7C00 + 512 = 0x7E00）に配置する
;************************************************************************
font:                                       ; フォント
.seg:   dw      0
.off:   dw      0
acpi_data:                                  ; ACPI data
.adr:   dd      0                           ; ACPI data address
.len:   dd      0                           ; ACPI data length

;************************************************************************
;   モジュール（先頭512バイト以降に配置）
;************************************************************************
%include    "modules/real/itoa.s"
%include    "modules/real/get_drive_param.s"
%include    "modules/real/get_font_adr.s"
%include    "modules/real/get_mem_info.s"
%include    "modules/real/kbc.s"
%include    "modules/real/lba_chs.s"
%include    "modules/real/read_lba.s"

;************************************************************************
;	ブート処理の第2ステージ
;************************************************************************
stage_2:
        cdecl   puts, .s0                       ; puts(.s0);

        ;--------------------------------
        ; ドライブ情報を取得
        ;--------------------------------
        cdecl   get_drive_param, boot           ; AX = get_drive_param(boot);
        cmp     ax, 0
        jne     .10E                            ; if (AX != 0) goto .10E
.10T:
        cdecl   puts, .e0                       ; puts(.e0);
        hlt
        jmp     $                               ; while (1) ;   // 無限ループ

.10E:
        ;--------------------------------
        ; ドライブ情報を表示
        ;--------------------------------
        mov     ax, [boot + drive.no]           ; AX = ブートドライブ
        cdecl   itoa, ax, .p1, 2, 16, 0b0100    ;
        mov     ax, [boot + drive.cyln]         ;
        cdecl   itoa, ax, .p2, 4, 16, 0b0100    ;
        mov     ax, [boot + drive.head]         ; AX = ヘッド数;
        cdecl   itoa, ax, .p3, 2, 16, 0b0100    ;
        mov     ax, [boot + drive.sect]         ; AX = トラックあたりのセクタ数;
        cdecl   itoa, ax, .p4, 2, 16, 0b0100    ;
        cdecl   puts, .s1

        jmp     stage_3                         ; 次のステージへ移行

        ;--------------------------------
        ; データ
        ;--------------------------------
.s0:    db  "2nd stage...", 0x0A, 0x0D, 0

.s1:    db  " Drive:0x"
.p1:    db  "  , C:0x"
.p2:    db  "    , H:0x"
.p3:    db  "  , S:0x"
.p4:    db  "  ", 0x0A, 0x0D, 0

.e0:    db  "Can't get drive parameter.", 0x0A, 0x0D, 0

;************************************************************************
;	ブート処理の第3ステージ
;************************************************************************
stage_3:
        cdecl   puts, .s0                       ; puts(.s0);

        ;--------------------------------
        ; プロテクトモードで使用するフォントは、
        ; BIOSに内蔵されたものを流用する
        ;--------------------------------
        cdecl   get_font_adr, font              ; // BIOSのフォントアドレスを取得

        ;--------------------------------
        ; フォントアドレスの表示
        ;--------------------------------
        cdecl   itoa, word [font.seg], .p1, 4, 16, 0b0100
        cdecl   itoa, word [font.off], .p2, 4, 16, 0b0100
        cdecl   puts, .s1

        ;--------------------------------
        ; メモリ情報の取得と表示
        ;--------------------------------
        cdecl   get_mem_info                    ; get_mem_info();

        mov     eax, [acpi_data.adr]            ; EAX = acpi_data.adr;
        cmp     eax, 0                          ;
        je      .10E                            ; if (EAX == 0) goto .10E;

        cdecl   itoa, ax, .p4, 4, 16, 0b0100    ; itoa(AX);     // 下位アドレスを変換
        shr     eax, 16                         ; EAX >>= 16;
        cdecl   itoa, ax, .p3, 4, 16, 0b0100    ; itoa(AX);     // 上位アドレスを変換

        cdecl   puts, .s2                       ; puts(.s2);    // アドレスを表示

.10E:
        ;--------------------------------
        ; 次のステージへ移行
        ;--------------------------------
        jmp     stage_4                         ; goto stage_4;

        ;--------------------------------
        ; データ
        ;--------------------------------
.s0:    db  "3rd stage...", 0x0A, 0x0D, 0

.s1:    db  " Font Address="
.p1:    db  "ZZZZ:"
.p2:    db  "ZZZZ", 0x0A, 0x0D, 0
        db  0x0A, 0x0D, 0

.s2:    db  " ACPI data="
.p3:    db  "ZZZZ"
.p4:    db  "ZZZZ", 0x0A, 0x0D, 0

;************************************************************************
;	ブート処理の第4ステージ
;************************************************************************
stage_4:
        cdecl   puts, .s0                       ; puts(.s0);

        ;--------------------------------
        ; A20ゲートの有効化
        ;--------------------------------
        cli                                     ; // 割り込み禁止

        cdecl   KBC_Cmd_Write, 0xAD             ; // キーボード無効化

        cdecl   KBC_Cmd_Write, 0xD0             ; // 出力ポート読み出しコマンド
        cdecl   KBC_Data_Read, .key             ; // 出力ポートデータ

        mov     bl, [.key]                      ; BL  = key;
        or      bl, 0x02                        ; BL |= 0x02; // A20ゲート有効化

        cdecl   KBC_Cmd_Write, 0xD1             ; // 出力ポート書き込みコマンド
        cdecl   KBC_Data_Write, bx              ; // 出力ポートデータ

        cdecl   KBC_Cmd_Write, 0xAE             ; // キーボード有効化

        sti                                     ; // 割り込み許可


        cdecl   puts, .s1                       ; puts(.s1);

        ;--------------------------------
        ; 次のステージへ移行
        ;--------------------------------
        jmp     stage_5                         ; goto stage_5;

        ;--------------------------------
        ; データ
        ;--------------------------------
.s0:    db  "4th stage...", 0x0A, 0x0D, 0
.s1:    db  " A20 Gate Enabled.", 0x0A, 0x0D, 0

.key:   dw  0

;************************************************************************
;	ブート処理の第5ステージ
;************************************************************************
stage_5:
        cdecl   puts, .s0                       ; puts(.s0);

        ;--------------------------------
        ; カーネルを 0x9C00 にロード
        ;--------------------------------
        cdecl   read_lba, boot, BOOT_SECT, KERNEL_SECT, BOOT_END

        cmp     ax, KERNEL_SECT
        jz      .success                        ; if (ax == KERNEL_SECT) goto .success;

.error:
        cdecl   puts, .e0
        jmp     $

.success:

        ;--------------------------------
        ; 次のステージへ移行
        ;--------------------------------
        jmp     stage_6                         ; goto stage_6;

        ;--------------------------------
        ; データ
        ;--------------------------------
.s0:    db  "5th stage...", 0x0A, 0x0D, 0
.e0:    db  "Failure load kernel...", 0x0A, 0x0D, 0

;************************************************************************
;	ブート処理の第6ステージ
;************************************************************************
stage_6:
        cdecl   puts, .s0                       ; puts(.s0);

        ;--------------------------------
        ; ビデオモードを設定する
        ;--------------------------------
        ;mov     ax, 0x0012                      ; VGA 640x480
        ;int     0x10                            ; BIOS(0x10, 0x12); // ビデオモードの設定
        mov     bx, 0x4000
        add     bx, VBE_MODE
        mov     ax, 0x4f02
        int     0x10

        ;--------------------------------
        ; 次のステージへ移行
        ;--------------------------------
        jmp     stage_7                         ; goto stage_7;

        ;--------------------------------
        ; データ
        ;--------------------------------
.s0:    db  "6th stage...", 0x0A, 0x0D, 0

;************************************************************************
;	GLOBAL DESCRIPTOR TABLE
;	(セグメントディスクリプタの配列)
;************************************************************************
;
;   セグメントディスクリプタ
;
;        +--------+-----------------: Base (0xBBbbbbbb)
;        |   +----|--------+--------: Limit(0x000Lllll)
;        |   |    |        |
;       +--+--+--+--+--+--+--+--+
;       |B |FL|f |b       |l    |
;       +--+--+--+--+--+--+--+--+
;           |  |                        76543210
;           |  +--------------------: f:PDDSTTTA
;           |                          P:Exist
;           |                          D:DPL(特権)
;           |                          S:(DT)0=システムorゲート, 1=データセグメント
;           |                          T:Type
;           |                            000(0)=R/- DATA
;           |                            001(1)=R/W DATA
;           |                            010(2)=R/- STACK
;           |                            011(3)=R/W STACK
;           |                            100(4)=R/- CODE
;           |                            101(5)=R/W CODE
;           |                            110(6)=R/- CONFORM
;           |                            111(7)=R/W CONFORM
;           |                          A:Accessed
;           |                       
;           +-----------------------: F:GD0ALLLL
;                                      G:Limit Scale(0=1, 1=4K)
;                                      D:Data/BandDown(0=16, 1=32Bit セグメント)
;                                      A:any
;                                      L:Limit[19:16]
ALIGN 4, db 0
;					  B_ F L f T b_____ l___
gdt:			dq	0x00_0_0_0_0_000000_0000	; NULL
.cs:			dq	0x00_C_F_9_A_000000_FFFF	; CODE 4G
.ds:			dq	0x00_C_F_9_2_000000_FFFF	; DATA 4G
.gdt_end:

;===============================================
;	セレクタ
;===============================================
sel_code	equ	.cs - gdt						; コード用セレクタ
sel_data	equ	.ds - gdt						; データ用セレクタ

;===============================================
;	GDT
;===============================================
gdtr:	dw 		gdt.gdt_end - gdt - 1			; ディスクリプタテーブルのリミット
		dd 		gdt								; ディスクリプタテーブルのアドレス

;===============================================
;	IDT（疑似：割り込み禁止にする為）
;===============================================
idtr:	dw 		0								; idt_limit
		dd 		0								; idt location

;************************************************************************
;	ブート処理の第7ステージ
;************************************************************************
stage_7:
        cdecl   puts, .s0                       ; puts(.s0);

        cli                                     ; 割込み禁止

		;---------------------------------------
		; GDTロード
		;---------------------------------------
		lgdt	[gdtr]							; // グローバルディスクリプタテーブルをロード
		lidt	[idtr]							; // 割り込みディスクリプタテーブルをロード

		;---------------------------------------
		; プロテクトモードへ移行
		;---------------------------------------
		mov		eax,cr0							; // PEビットをセット
		or		ax, 1							; CR0 |= 1;
		mov		cr0,eax							; 

		jmp		$ + 2							; 先読みをクリア

		;---------------------------------------
		; セグメント間ジャンプ
		;---------------------------------------
[BITS 32]
		DB		0x66							; オペランドサイズオーバーライドプレフィックス
		jmp		sel_code:code_32

.s0:    db  "7th stage...", 0x0A, 0x0D, 0

;************************************************************************
;	32ビットコード開始
;************************************************************************
code_32:

		;---------------------------------------
		; セレクタを初期化
		;---------------------------------------
		mov		ax, sel_data					;
		mov		ds, ax							;
		mov		es, ax							;
		mov		fs, ax							;
		mov		gs, ax							;
 		mov		ss, ax							;

		;---------------------------------------
		; カーネル部をコピー
		;---------------------------------------
		mov		ecx, (KERNEL_SIZE) / 4			; ECX = 4バイト単位でコピー;
		mov		esi, BOOT_END					; ESI = 0x0000_9C00; // カーネル部
		mov		edi, KERNEL_LOAD				; EDI = 0x0010_1000; // 上位メモリ
		cld										; // DFクリア（+方向）
		rep movsd								; while (--ECX) *EDI++ = *ESI++;

		;---------------------------------------
		; カーネル処理に移行
		;---------------------------------------
        mov     esp, KERNEL_LOAD                ; スタックポインタをカーネルのものにする
		jmp		KERNEL_LOAD						; カーネル
        
;************************************************************************
;   パティング（このファイルは8KBとする）
;************************************************************************
        times BOOT_SIZE - ($ - $$) db 0    ; 8KB