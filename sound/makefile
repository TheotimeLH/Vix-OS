all : run

boot.o: boot.s
	i686-elf-as --32 boot.s -o boot.o

sound_driver.o : sound_driver.c
	i686-elf-gcc -m32 -c sound_driver.c -o sound_driver.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

VixOS.bin : linker.ld sound_driver.o boot.o ../common/common.o ../video/vga_driver.o
	i686-elf-ld -T $^ -o VixOS.bin -nostdlib

check-multiboot : VixOS.bin
	grub-file --is-x86-multiboot VixOS.bin

VixOS.iso : VixOS.bin grub.cfg check-multiboot
	mkdir -p isodir/boot/grub
	cp VixOS.bin isodir/boot/VixOS.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o VixOS.iso isodir

run : VixOS.iso
	qemu-system-i386 -soundhw all -cdrom VixOS.iso

clean: 
	rm *.o *.bin *.iso 
	rm -fr isodir

./PHONY : run check-multiboot all
