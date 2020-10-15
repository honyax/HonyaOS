#include "define.h"

void write_mem8(unsigned int addr, unsigned char data) {
    unsigned char *p_addr = (unsigned char *) addr;
    *p_addr = data;

    return;
}

void hmemset(void *addr, unsigned char c, int size) {
    unsigned char *p_addr = (unsigned char *) addr;
    for (int i = 0; i < size; i++) {
        *p_addr = c;
        p_addr++;
    }

    return;
}
