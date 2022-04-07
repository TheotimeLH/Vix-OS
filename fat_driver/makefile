all:boot.o linker.ld fat_driver.o makefile ../common/common.o ../video/vga_driver.o main.o ../ata_driver/ata_driver.o
	i686-elf-g++ -T linker.ld boot.o main.o fat_driver.o ../video/vga_driver.o ../ata_driver/ata_driver.o ../common/common.o -nostdlib -ffreestanding

boot.o:boot.s
	i686-elf-as -o boot.o boot.s

clean:
	rm -f *.o a.out

run:all
	qemu-system-i386 -kernel a.out -drive file=test,index=0,media=disk,format=raw

../common/common.o:../common/common.c
	make --directory=../common

../video/vga_driver.o:../video/vga_driver.c
	make --directory=../video

../ata_driver/ata_driver.o:../ata_driver/ata_driver.c
	make --directory=../ata_driver

%.o:%.cpp
	i686-elf-g++ -o $@ -c $< -ffreestanding  -O2 -Wall -Wextra -fno-exceptions -fno-rtti
