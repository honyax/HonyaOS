BOOT_BIN	= boot.bin

.PHONY: all
all:
	nasm.exe boot.asm -o $(BOOT_BIN) -l boot.lst

.PHONY: clean
clean:
	rm -f $(BOOT_BIN)
	rm -f boot.lst
