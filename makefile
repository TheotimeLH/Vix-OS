#On fait ici un makefile qui va tout compiler et lancer le noyau comme il est \
	pour l'instant tout se trouve dans test_entry


video :
	cd video && make vga_driver.o

run : 
	qemu-system-i386 -soundhw all -cdrom VixOS.iso

debug :
	qemu-system-i386 -s -S -soundhw all -cdrom VixOS.iso

VixOS.bin : linker.ld video/vga_driver.o 
	i686-elf-gcc -ggdb -T $^ -o VixOS.bin -nostdlib
	grub-file --is-x86-multiboot VixOS.bin

clean :
	rm *.o *.iso
	rm -fr isodir

.PHONY : all run clean
