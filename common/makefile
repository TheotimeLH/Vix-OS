all : common.o descriptor_tables.o interrupt.o isr.o gdt_flush.o

common.o : common.c common.h
	i686-elf-gcc -m32 -c common.c -o common.o -ffreestanding -O2 -Wall -Wextra
descriptor_tables.o: descriptor_tables.c descriptor_tables.h 
	i686-elf-gcc -m32 -c $< -o $@ -ffreestanding -O2 -Wall -Wextra
interrupt.o : interrupt.s
	i686-elf-as --32 interrupt.s -o interrupt.o
isr.o : isr.c isr.h
	i686-elf-gcc -m32 -c isr.c -o isr.o -ffreestanding -O2 -Wall -Wextra
gdt_flush.o : gdt_flush.s
	i686-elf-as --32 gdt_flush.s -o gdt_flush.o

clean : 
	rm *.o

.PHONY : clean
