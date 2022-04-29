#On fait ici un makefile qui va tout compiler et lancer le noyau comme il est \
	pour l'instant tout se trouve dans test_entry
COMMON_INCLUDE_SRC =$(wildcard common/*.c)
COMMON_INCLUDE =$(COMMON_INCLUDE_SRC:.c=.o)
COMMON_INCLUDE_ASM =$(wildcard common/*.s)
COMMON_INCLUDE_ = $(COMMON_INCLUDE_ASM:.s=.o)



all : run
common :  # On appeler le makefile de common
	cd common && make

video :
	cd common && make

kernel : 
	cd kernel && make

%.o : %.c
	i686-elf-gcc -ggdb -m32 -c $< -o $@ -std=gnu99 -ffreestanding -O2

%.o : %.s
	i686-elf-as -ggdb --32 $< -o $@

run : VixOS.iso
	qemu-system-i386 -soundhw all -cdrom VixOS.iso

debug : VixOS.iso
	qemu-system-i386 -s -S -soundhw all -cdrom VixOS.iso

VixOS.bin : linker.ld  $(COMMON_INCLUDE) $(COMMON_INCLUDE_) video/vga_driver.o boot.o kernel/main.o
	i686-elf-gcc -ggdb -T $^ -o VixOS.bin -nostdlib
	grub-file --is-x86-multiboot VixOS.bin

VixOS.iso : common video kernel VixOS.bin grub.cfg 
	mkdir -p isodir/boot/grub
	cp VixOS.bin isodir/boot/VixOS.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o VixOS.iso isodir

clean :
	rm *.o *.iso
	rm -fr isodir

.PHONY : all run clean common
