gcc.exe -c kernel.c
objconv.exe -fnasm kernel.o kernel_base.tmp.s
nasm.exe kernel.tmp.s -o kernel.bin -l kernel.lst
nasm.exe boot.s -o boot.bin -l boot.lst
cat boot.bin kernel.bin > boot.img
