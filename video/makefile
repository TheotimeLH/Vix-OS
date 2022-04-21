all : run

boot.o : boot.s
	i686-elf-as -ggdb --32 boot.s -o boot.o

vga_driver.o : vga_driver.c
	i686-elf-gcc -ggdb -m32 -c vga_driver.c -o vga_driver.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	objcopy --only-keep-debug vga_driver.o vga.debug

VixOS.bin : linker.ld vga_driver.o boot.o ../common/common.o ../sound/sound_driver.o ../common/descriptor_tables.o ../common/interrupt.o ../common/isr.o ../common/gdt_flush.o
	i686-elf-gcc -ggdb -T $^ -o VixOS.bin -nostdlib
	grub-file --is-x86-multiboot VixOS.bin

VixOS.iso : VixOS.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp VixOS.bin isodir/boot/VixOS.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o VixOS.iso isodir

run : VixOS.iso
	qemu-system-i386 -s -S -soundhw all -cdrom VixOS.iso

clean : 
	rm *.o *.iso
	rm -fr isodir

.PHONY: all run clean

