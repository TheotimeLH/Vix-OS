vga_driver.o : vga_driver.c
	i686-elf-gcc -ggdb -m32 -c vga_driver.c -o vga_driver.o -std=gnu99 -ffreestanding -Wall -Wextra
	objcopy --only-keep-debug vga_driver.o vga.debug

clean : 
	rm -f *.o *.iso
	rm -fr isodir

.PHONY: clean

