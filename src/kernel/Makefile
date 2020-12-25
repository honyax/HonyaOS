KERNEL_BIN	= kernel.bin
OBJECTS		= *.o

.PHONY: all
all: $(OBJECTS)
	ld -m elf_i386 -o $(KERNEL_BIN) -T linkerscript -Map kernel.map $(OBJECTS)

$(OBJECTS): *.c *.s *.h ../lib/*.c ../lib/*.h ../syscall/*.s ../syscall/*.h
	gcc -c -m32 -fno-pic -fno-stack-protector *.c ../lib/*.c
	gcc -c -m32 *.s ../syscall/*.s

.PHONY: clean
clean:
	rm -f $(KERNEL_BIN)
	rm -f *.o
	rm -f kernel.map
