void fuga();

int kernel_main() {
    int aaa = 0x12345678;
    fuga(aaa);
    return aaa;
}

void fuga(int param) {
loop:
    goto loop;
}
