all : common.o idt.o interrupt.o isr.o

common.o : common.c common.h
	i686-elf-gcc -m32 -c common.c -o common.o -ffreestanding -O2 -Wall -Wextra
idt.o: idt.h idt.c
	i686-elf-gcc -m32 -c idt.c -o idt.o -ffreestanding -O2 -Wall -Wextra
interrupt.o : interrupt.s
	i686-elf-as --32 interrupt.s -o interrupt.o
isr.o : isr.c isr.h
	i686-elf-gcc -m32 -c isr.c -o isr.o -ffreestanding -O2 -Wall -Wextra


clean : 
	rm *.o

.PHONY : clean
