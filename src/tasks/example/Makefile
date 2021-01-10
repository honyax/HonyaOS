PROG	= example

HNY		= ../$(PROG).hny
OBJECTS	= *.o

.PHONY: all
all: $(OBJECTS)
	ld -m elf_i386 -o $(HNY) -T ../linkerscript -Map $(PROG).map $(OBJECTS)

$(OBJECTS): *.c ../../lib/* ../../syscall/*
	gcc -c -m32 -fno-pic -fno-stack-protector *.c ../_.c ../../lib/*.c
	gcc -c -m32 ../../syscall/*.s

.PHONY: clean
clean:
	rm -f $(HNY)
	rm -f *.o
	rm -f *.map
