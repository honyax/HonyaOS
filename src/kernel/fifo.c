#include "define.h"

void fifo32_init(FIFO32 *fifo, int size, int *buf) {
    fifo->buf = buf;
    fifo->size = size;
    fifo->pos_r = 0;
    fifo->pos_w = 0;
    fifo->len = 0;
}

// FIFO32にデータを追加
// return -1 : FIFOがいっぱいでデータ追加失敗
//        その他 : FIFOに溜まっている未読のデータ数
int fifo32_put(FIFO32 *fifo, int data) {

    if (fifo->size <= fifo->len)
        return -1;

    fifo->buf[fifo->pos_w] = data;
    fifo->pos_w++;
    if (fifo->size <= fifo->pos_w) {
        fifo->pos_w = 0;
    }
    fifo->len++;

    return fifo->len;
}

// FIFO32からデータを取得
// return -1 : FIFOにデータが無い
//        その他 : FIFOに設定されていた最初のデータ
int fifo32_get(FIFO32 *fifo) {

    if (fifo->len <= 0)
        return -1;
    
    int data = fifo->buf[fifo->pos_r];
    fifo->pos_r++;
    if (fifo->size <= fifo->pos_r) {
        fifo->pos_r = 0;
    }
    fifo->len--;

    return data;
}
