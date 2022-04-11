all:myos.bin

myos.bin:linker.ld kernel.o boot.o ata_driver.o ../common/common.o ../video/vga_driver.o
	i686-elf-gcc -T linker.ld -o myos.bin boot.o kernel.o ata_driver.o ../common/common.o ../video/vga_driver.o -ffreestanding -nostdlib

boot.o:boot.s
	i686-elf-as -o boot.o boot.s

kernel.o:kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o -ffreestanding

ata_driver.o:ata_driver.c
	i686-elf-gcc -c ata_driver.c -o ata_driver.o -ffreestanding

../common/common.o:../common/common.c
	make --directory=../common

../video/vga_driver.o:../video/vga_driver.c
	make --directory=../video

run:all
	qemu-system-i386 -kernel myos.bin -drive file=test,index=0,media=disk,format=raw

clean:
	rm -f myos.bin boot.o kernel.o ata_driver.o
