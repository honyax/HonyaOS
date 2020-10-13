BOOT_IMG	= boot.img
BOOT_BIN	= boot/boot.bin
KERNEL_BIN	= kernel/kernel.bin
ZERO_BIN	= zero.bin

BOCHS		= "/mnt/c/Program Files (x86)/Bochs-2.6.9/bochsdbg.exe"

.PHONY: all
all: $(BOOT_BIN) $(KERNEL_BIN) $(ZERO_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) $(ZERO_BIN) > $(BOOT_IMG)

$(BOOT_BIN):
	cd boot; make

$(KERNEL_BIN):
	cd kernel; make

$(ZERO_BIN):
	dd if=/dev/zero of=./zero.bin bs=1 count=8192 > /dev/null 2>/dev/null

.PHONY: boot
boot:
	$(BOCHS) -q -f ../env/bochsrc.bxrc -rc ../env/cmd.init

.PHONY: clean
clean:
	rm -f $(BOOT_IMG)
	rm -f $(ZERO_BIN)
	cd boot; make clean
	cd kernel; make clean
