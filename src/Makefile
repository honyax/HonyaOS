BOOT_IMG	= boot.img
BOOT_BIN	= boot/boot.bin
KERNEL_BIN	= kernel/kernel.bin
ZERO_BIN	= zero.bin

BOCHS		= "bochsdbg.exe"
QEMU		= "qemu-system-i386.exe"

.PHONY: all
all: $(BOOT_IMG)

$(BOOT_IMG): $(BOOT_BIN) $(KERNEL_BIN) $(ZERO_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) $(ZERO_BIN) > $(BOOT_IMG)

$(BOOT_BIN):
	cd boot; make

$(KERNEL_BIN):
	cd kernel; make

$(ZERO_BIN):
	dd if=/dev/zero of=./zero.bin bs=1 count=8192 > /dev/null 2>/dev/null

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
	rm -f $(ZERO_BIN)
	cd boot; make clean
	cd kernel; make clean
