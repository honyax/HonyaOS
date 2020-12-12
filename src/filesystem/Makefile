FS_BIN	= filesystem.bin

.PHONY: all
all:
	nasm.exe filesystem.asm -o $(FS_BIN) -l filesystem.lst

.PHONY: clean
clean:
	rm -f $(FS_BIN)
	rm -f *.lst
