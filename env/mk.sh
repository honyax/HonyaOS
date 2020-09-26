if [ -e kernel.c ]; then

  # kernel.c があればコンパイル＆NASMを生成してアセンブル＆結合
  echo "kernel.c exists."
  nasm.exe boot.s -o boot.bin -l boot.lst
  rm -f kernel.tmp.c
  cat *.c > kernel.tmp.c
  gcc.exe -c kernel.tmp.c
  objconv.exe -fnasm kernel.tmp.o kernel_base.tmp.s
  convert.sh kernel_base.tmp.s kernel_template.s > kernel.tmp.s
  nasm.exe kernel.tmp.s -o kernel.bin -l kernel.lst
  cat boot.bin kernel.bin > boot.img

else

  # kernelがない場合はそのままboot.sをアセンブル
  echo "kernel not exists."
  nasm.exe boot.s -o boot.img -l boot.lst

fi

