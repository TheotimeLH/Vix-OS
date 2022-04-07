all : common.o idt.o

common.o : common.c common.h
	i686-elf-gcc -m32 -c common.c -o common.o -ffreestanding -O2 -Wall -Wextra
idt.o: idt.h idt.c
	i686-elf-gcc -m32 -c idt.c -o idt.o -ffreestanding -O2 -Wall -Wextra

clean : 
	rm *.o

.PHONY : clean
