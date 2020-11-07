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

unsigned int *kernel_page_dir = (unsigned int *) KERNEL_PAGE_DIR;
unsigned int *kernel_page_table = (unsigned int *) KERNEL_PAGE_TABLE;

void init_paging()
{

#if 0
    // 4MB分のメモリのpaging設定を行う

    // ページディレクトリは、先頭だけ設定し、他は全て 0
    kernel_page_dir[0] = KERNEL_PAGE_TABLE | PDE_FLAGS_P | PDE_FLAGS_RW | PDE_FLAGS_US;
    for (int i = 1; i < 1024; i++) {
        kernel_page_dir[i] = 0;
    }

    // ページテーブルは、メモリアドレス 0 ～ 4MB までの領域を指す
    for (int i = 0; i < 1024; i++) {
        kernel_page_table[i] = 4096 * i | PTE_FLAGS_P | PTE_FLAGS_RW | PTE_FLAGS_US;
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
    _set_cr3((unsigned int)kernel_page_dir);

    // CR0のページング使用フラグをON
    unsigned int cr0 = _get_cr0();
    cr0 |= CR0_PAGING_FLAG;
    _set_cr0(cr0);
}
