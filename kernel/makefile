common = ../common/common.o ../common/descriptor_tables.o ../common/interrupt.o ../common/gdt_flush.o ../common/isr.o ../common/timer.o
other = ../fat_driver/fat_driver.o ../ata_driver/ata_driver.o ../video/vga_driver.o
kernel=process.o main.o boot.o

a.out:linker.ld makefile $(kernel)  $(common) $(other)
	i686-elf-g++ -T linker.ld $(kernel) $(common) $(other) -nostdlib -ffreestanding

boot.o:boot.s
	i686-elf-as -o boot.o boot.s

clean:
	rm -f *.o a.out

run:a.out
	qemu-system-i386 -kernel a.out -drive file=test,index=0,media=disk,format=raw -m 1G

../common/%.o:../common/%.c ../common/%.h
	make --directory=../common

../common/interrupt.o:../common/interrupt.s
	make --directory=../common

../common/gdt_fush.o:../common/interrupt.s
	make --directory=../common


../video/vga_driver.o:../video/vga_driver.c
	make --directory=../video

../fat_driver/fat_driver.o:../fat_driver/fat_driver.cpp
	make --directory=../fat_driver

../ata_driver/%.o:../ata_driver/%.cpp
	make --directory=../ata_driver

%.o:%.cpp
	i686-elf-g++ -o $@ -c $< -ffreestanding  -Wall -Wextra -fno-exceptions -fno-rtti

prog:prog_test/prog.s
	i686-elf-as -o prog_test/prog.o prog_test/prog.s
	i686-elf-gcc -o prog_test/prog prog_test/prog.o -nostdlib
	sudo mount test /mnt/test
	sudo cp prog_test/prog /mnt/test
	sudo umount test
	
	
