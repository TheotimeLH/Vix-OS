#assemble boot.s file
as --32 boot.s -o boot.o

#compile vga.c file
gcc -m32 -c vga.c -o vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#linking the vga with vga.o and boot.o files
ld -m elf_i386 -T linker.ld vga.o boot.o -o VixOS.bin -nostdlib

#check VixOS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot VixOS.bin

#building the iso file
mkdir -p isodir/boot/grub
cp VixOS.bin isodir/boot/VixOS.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o VixOS.iso isodir

#run it in qemu
qemu-system-x86_64 -cdrom VixOS.iso
