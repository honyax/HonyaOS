EXAMPLE_HNY	= example.hny
OBJECTS		= *.o

.PHONY: all
all: $(OBJECTS)
	ld -m elf_i386 -o $(EXAMPLE_HNY) -T linkerscript -Map example.map $(OBJECTS)

$(OBJECTS): *.c ../lib/*.c ../lib/*.h ../syscall/*.s ../syscall/*.h
	gcc -c -m32 -fno-pic -fno-stack-protector *.c ../lib/*.c
	gcc -c -m32 ../syscall/*.s

.PHONY: clean
clean:
	rm -f $(EXAMPLE_HNY)
	rm -f *.o
	rm -f *.map
