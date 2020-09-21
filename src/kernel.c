void fuga();

int kernel_main() {
    int aaa = 0x11335577;
    fuga(aaa);
    return aaa;
}

void fuga(int param) {
loop:
    goto loop;
}
