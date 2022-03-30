all:myos.bin

myos.bin:linker.ld kernel.o boot.o ata_driver.o
	i686-elf-gcc -T linker.ld -o myos.bin boot.o kernel.o ata_driver.o -ffreestanding -nostdlib

boot.o:boot.s
	i686-elf-as -o boot.o boot.s

kernel.o:kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o -ffreestanding

ata_driver.o:ata_driver.c
	i686-elf-gcc -c ata_driver.c -o ata_driver.o -ffreestanding

run:all
	qemu-system-i386 -kernel myos.bin

clean:
	rm -f myos.bin boot.o kernel.o ata_driver.o
