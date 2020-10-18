KERNEL_BIN	= kernel.bin
OBJECTS		= *.o

.PHONY: all
all: $(OBJECTS)
	ld -m elf_i386 -o $(KERNEL_BIN) -T linkerscript -Map kernel.map $(OBJECTS)

$(OBJECTS):
	gcc -c -m32 -fno-pic -fno-stack-protector *.c
	gcc -c -m32 *.s

.PHONY: clean
clean:
	rm -f $(KERNEL_BIN)
	rm -f *.o
