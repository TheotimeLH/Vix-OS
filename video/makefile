vga_driver.o : vga_driver.c
	i686-elf-gcc -ggdb -m32 -c vga_driver.c -o vga_driver.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	objcopy --only-keep-debug vga_driver.o vga.debug

clean : 
	rm *.o *.iso
	rm -fr isodir

.PHONY: clean

