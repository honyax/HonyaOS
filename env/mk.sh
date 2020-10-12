cd boot
nasm.exe boot.asm -o boot.bin -l boot.lst
cd ..

# no-pic, no-stack-protectorを指定しないとリンクエラーが出る
cd kernel
gcc -c -m32 -fno-pic -fno-stack-protector *.c

# アセンブラコードもアセンブルしておく
gcc -c -m32 *.s

# kernel.oが先頭じゃないといけない。-eで指定してもELF形式じゃないから無駄
#ld -m elf_i386 -e kernel_main -o kernel.bin -T linkerscript -Map kernel.map kernel.o graphic.o sprintf.o
ld -m elf_i386 -o kernel.bin -T linkerscript -Map kernel.map kernel.o graphic.o sprintf.o sample.o
cd ..

# 現在は強制的にカーネル用に8KBロードしているのでゼロ埋め。そのうち何とかしたい
dd if=/dev/zero of=./zero.bin bs=1 count=8192 > /dev/null 2>/dev/null
cat boot/boot.bin kernel/kernel.bin zero.bin > boot.img



# 以下は昔の手法
# if [ -e kernel.c ]; then
# 
#   # kernel.c があればコンパイル＆NASMを生成してアセンブル＆結合
#   echo "kernel.c exists."
#   nasm.exe boot.s -o boot.bin -l boot.lst
# 
#   # C言語のソースファイルを全て結合してコンパイルする
#   # 但し、kernel.cが先頭になるように留意する
#   cat kernel.c > kernel.tmp.c
#   for F in $(find . -name "*.c" | grep -v kernel); do
#     cat $F >> kernel.tmp.c
#   done
#   gcc.exe -c kernel.tmp.c
#   objconv.exe -fnasm kernel.tmp.o kernel_base.tmp.s
#   convert.sh kernel_base.tmp.s kernel_template.s > kernel.tmp.s
#   nasm.exe kernel.tmp.s -o kernel.bin -l kernel.lst
#   cat boot.bin kernel.bin > boot.img
# 
# else
# 
#   # kernelがない場合はそのままboot.sをアセンブル
#   echo "kernel not exists."
#   nasm.exe boot.s -o boot.img -l boot.lst
# 
# fi

