#include "define.h"

#define PTE_FLAGS_P                 0x00000001
#define PTE_FLAGS_RW                0x00000002
#define PTE_FLAGS_US                0x00000004
#define PTE_FLAGS_PWT               0x00000008
#define PTE_FLAGS_PCD               0x00000010
#define PTE_FLAGS_A                 0x00000020
#define PTE_FLAGS_D                 0x00000040
#define PTE_FLAGS_PAT               0x00000080
#define PTE_FLAGS_G                 0x00000100
#define PTE_FLAGS_AVAILABLE         0x00000E00
#define PTE_FRAME_ADDRESS           0xFFFFF000

#define PDE_FLAGS_P                 0x00000001
#define PDE_FLAGS_RW                0x00000002
#define PDE_FLAGS_US                0x00000004
#define PDE_FLAGS_PWT               0x00000008
#define PDE_FLAGS_PCD               0x00000010
#define PDE_FLAGS_A                 0x00000020
#define PDE_FLAGS_RESERVED          0x00000040
#define PDE_FLAGS_PS                0x00000080
#define PDE_FLAGS_G                 0x00000100
#define PDE_FLAGS_AVAILABLE         0x00000E00
#define PDE_PAGE_TABLE_ADDRESS      0xFFFFF000

#define CR0_PAGING_FLAG             0x80000000

static uint *kernel_page_dir;
static uint *kernel_page_table;

void init_paging()
{
    kernel_page_dir = (uint *) KERNEL_PAGE_DIR;
    kernel_page_table = (uint *) KERNEL_PAGE_TABLE;

#if 1
    // 128MB分のメモリと、VRAM領域のメモリのpaging設定を行う

    // ページディレクトリは、ひとまず全て 0 で初期化
    for (int i = 0; i < 1024; i++) {
        kernel_page_dir[i] = 0;
    }

    // 128MB 用の設定
    for (int i = 0; i < 32; i++) {
        kernel_page_dir[i] = KERNEL_PAGE_TABLE + 0x1000 * i | PDE_FLAGS_P | PDE_FLAGS_RW | PDE_FLAGS_US;
    }

    // VRAM用のpagingを行うため、VRAMのアドレスから 4MB はそのまま移行させる
    // 通常は 1280 x 1024 x 8bit なので、4MB 分対応すれば十分
    uint vram_index = param_vram >> 22;
    kernel_page_dir[vram_index] = KERNEL_PAGE_TABLE + 0x1000 * 32 | PDE_FLAGS_P | PDE_FLAGS_RW | PDE_FLAGS_US;

    // ページテーブルは、メモリアドレス 0 ～ 128MB までの領域を指す
    for (int i = 0; i < 1024 * 32; i++) {
        kernel_page_table[i] = 4096 * i | PTE_FLAGS_P | PTE_FLAGS_RW | PTE_FLAGS_US;
    }

    // VRAM用のマッピングを設定
    for (int i = 0; i < 1024; i++) {
        kernel_page_table[1024 * 32 + i] = param_vram + 4096 * i | PTE_FLAGS_P | PTE_FLAGS_RW | PTE_FLAGS_US;
    }

#else
    // 4GB分のメモリのpaging設定を行う

    // 全ページディレクトリの設定
    for (int i = 0; i < 1024; i++) {
        kernel_page_dir[i] = KERNEL_PAGE_TABLE + 0x1000 * i | PDE_FLAGS_P | PDE_FLAGS_RW | PDE_FLAGS_US;
    }

    // 全ページテーブルの設定
    for (int i = 0; i < 1024 * 1024; i++) {
        kernel_page_table[i] = 4096 * i | PTE_FLAGS_P | PTE_FLAGS_RW | PTE_FLAGS_US;
    }

#endif

    // CR3にページディレクトリのアドレスを設定
    _set_cr3((uint)kernel_page_dir);

    // CR0のページング使用フラグをON
    uint cr0 = _get_cr0();
    cr0 |= CR0_PAGING_FLAG;
    _set_cr0(cr0);
}
