#assemble boot.s file
i686-elf-as --32 boot.s -o boot.o

#compile sound_driver.c file
i686-elf-gcc -m32 -c sound_driver.c -o sound_driver.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#linking the vga with vga.o and boot.o files
i686-elf-ld -T linker.ld sound_driver.o boot.o ../common/common.o ../video/vga_driver.o -o VixOS.bin -nostdlib

#check VixOS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot VixOS.bin

#building the iso file
mkdir -p isodir/boot/grub
cp VixOS.bin isodir/boot/VixOS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o VixOS.iso isodir

#run it in qemu
qemu-system-x86_64 -soundhw pcspk -cdrom VixOS.iso
