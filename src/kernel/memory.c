#include "define.h"

void write_mem8(uint addr, byte data)
{
    byte *p_addr = (byte *) addr;
    *p_addr = data;

    return;
}

byte read_mem8(uint addr)
{
    byte *p_addr = (byte *) addr;
    return *p_addr;
}

void hmemset(void *addr, byte c, int size)
{
    byte *p_addr = (byte *) addr;
    for (int i = 0; i < size; i++) {
        *p_addr = c;
        p_addr++;
    }

    return;
}

typedef struct
{
    uint start_addr;
    uint size;
} USED_MEMORY;

static uint allocatable_start_addr;
static uint total_size;
static uint alloc_error_count;
static uint free_error_count;

#define USED_MEMORY_MAX     1024
static USED_MEMORY* used_memories;

// メモリ初期化
void init_memory()
{
    // ヒープメモリとして 0x0020 0000 - 0x07FF 0000 の領域が与えられている
    // まずは管理領域として 0x0020 0000 から USED_MEMORY * USED_MEMORY_MAX 分の領域を確保
    used_memories = (USED_MEMORY*) HEAP_MEMORY_START;
    uint manage_memory_size = sizeof(USED_MEMORY) * USED_MEMORY_MAX;
    allocatable_start_addr = HEAP_MEMORY_START + manage_memory_size;
    total_size = HEAP_MEMORY_END - allocatable_start_addr;
    alloc_error_count = 0;
    free_error_count = 0;
    for (int i = 0; i < USED_MEMORY_MAX; i++) {
        used_memories[i].start_addr = 0;
        used_memories[i].size = 0;
    }
}

// メモリ確保
void *hmalloc(uint size)
{
    if (size <= 0) {
        return (void*) NULL;
    }

    // 確保可能なメモリ数を越えていたら確保失敗
    if (used_memories[USED_MEMORY_MAX - 1].size != 0) {
        alloc_error_count++;
        return (void*) NULL;
    }

    // スタート地点から探索して指定したsize分の空きがある箇所を探す
    uint alloc_addr = allocatable_start_addr;
    for (int i = 0; i < USED_MEMORY_MAX; i++) {
        if (used_memories[i].size == 0) {
            // 未使用の領域に到達したら、そこからメモリを確保
            if (alloc_addr + size < HEAP_MEMORY_END) {
                // ヒープ領域を超えていないか確認し、メモリ確保
                used_memories[i].start_addr = alloc_addr;
                used_memories[i].size = size;
                return (void*) alloc_addr;
            } else {
                // ヒープ領域を超えていたらヌルを返す
                alloc_error_count++;
                return (void*) NULL;
            }
        } else if (used_memories[i].start_addr - alloc_addr >= size) {
            // 次のメモリの開始アドレスまでの間にメモリを確保できるなら、以降の確保領域のインデックスを全て +1 してここに確保
            for (int j = USED_MEMORY_MAX - 1; j > i; j--) {
                if (used_memories[j - 1].size == 0) {
                    continue;
                }
                used_memories[j].start_addr = used_memories[j - 1].start_addr;
                used_memories[j].size = used_memories[j - 1].size;
            }
            used_memories[i].start_addr = alloc_addr;
            used_memories[i].size = size;
            return (void*) alloc_addr;
        }

        // 確保されている領域の最後にアドレスを移して次の領域を再探索
        alloc_addr = used_memories[i].start_addr + used_memories[i].size;
    }

    // メモリが最大数まで確保されている
    alloc_error_count++;
    return (void*) NULL;
}

void hfree(void *ptr)
{
    uint free_addr = (uint) ptr;
    for (int i = 0; i < USED_MEMORY_MAX; i++) {
        if (used_memories[i].start_addr == free_addr) {
            // 開放する領域が見つかったら、以降の確保領域のインデックスを全て -1
            for (int j = i; j < USED_MEMORY_MAX - 1; j++) {
                if (used_memories[j].size == 0) {
                    return;
                }
                used_memories[j].start_addr = used_memories[j + 1].start_addr;
                used_memories[j].size = used_memories[j + 1].size;
            }
        }
    }

    // ここに来たということは開放するアドレスが見つからなかったということ
    free_error_count++;
    return;
}
