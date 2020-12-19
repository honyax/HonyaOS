BOOT_IMG	= boot.img
BOOT_BIN	= boot/boot.bin
KERNEL_BIN	= kernel/kernel.bin
FS_IMG		= filesystem/fat12.img

BOCHS		= "bochsdbg.exe"
QEMU		= "qemu-system-i386.exe"

.PHONY: all
all: $(BOOT_IMG)

$(BOOT_IMG): $(BOOT_BIN) $(KERNEL_BIN) $(FS_IMG)
	cat $(BOOT_BIN) $(KERNEL_BIN) $(FS_IMG) > $(BOOT_IMG)

$(BOOT_BIN): boot/*.asm boot/*.s boot/modules/*.s
	cd boot; make

$(KERNEL_BIN): kernel/*.c kernel/*.s lib/*.c
	cd kernel; make

$(FS_IMG): filesystem/*.asm
	cd filesystem; make

.PHONY: run
run: $(BOOT_IMG)
	$(BOCHS) -q -f ../env/bochsrc.bxrc -rc ../env/cmd.init

.PHONY: dbg
dbg: $(BOOT_IMG)
	$(BOCHS) -q -f ../env/bochsrc.bxrc

.PHONY: qemu
qemu: $(BOOT_IMG)
	$(QEMU) -rtc base=localtime -drive file=boot.img,format=raw -boot order=c

.PHONY: clean
clean:
	rm -f $(BOOT_IMG)
	cd boot; make clean
	cd kernel; make clean
	cd filesystem; make clean
