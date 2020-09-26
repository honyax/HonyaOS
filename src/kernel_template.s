;************************************************************************
;
;	HonyaOS kernelプログラム
;
;************************************************************************

;************************************************************************
;	マクロ
;************************************************************************
%include	"define.s"
%include	"macro.s"

		ORG		KERNEL_LOAD				    	; kernelのロードアドレスをアセンブラに指示

[BITS 32]
;************************************************************************
;	エントリポイント
;************************************************************************
kernel:
		;---------------------------------------
		; フォントアドレスを取得
		;---------------------------------------
		mov		esi, BOOT_LOAD + SECT_SIZE		; ESI   = 0x7C00 + 512
		movzx	eax, word [esi + 0]				; EAX   = [ESI + 0] // セグメント
		movzx	ebx, word [esi + 2]				; EBX   = [ESI + 2] // オフセット
		shl		eax, 4							; EAX <<= 4;
		add		eax, ebx						; EAX  += EBX;
		mov		[FONT_ADR], eax					; FONT_ADR[0] = EAX;

{KERNEL_SOURCECODE}

SECTION .text

ALIGN 4, db 0
FONT_ADR:	dd	0
RTC_TIME:	dd	0

;************************************************************************
;	モジュール
;************************************************************************
%include	"modules/protect/vga.s"
%include	"modules/protect/draw_char.s"

;************************************************************************
;	パディング
;************************************************************************
		times	KERNEL_SIZE - ($ - $$)	db	0	; パディング
