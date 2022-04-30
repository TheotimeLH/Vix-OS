all:crt0.o stdlib.o

crt0.o:crt0.s
	i686-elf-as -o crt0.o crt0.s

stdlib.o:stdlib.h stdlib.c
	i686-elf-gcc -c stdlib.c -nostdlib -ffreestanding

.PHONY : all