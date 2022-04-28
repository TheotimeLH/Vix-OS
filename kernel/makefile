common = ../common/common.o ../common/descriptor_tables.o ../common/interrupt.o ../common/gdt_flush.o ../common/isr.o ../common/timer.o

a.out:boot.o linker.ld makefile ../video/vga_driver.o main.o $(common)
	i686-elf-g++ -T linker.ld boot.o main.o ../video/vga_driver.o $(common) -nostdlib -ffreestanding

main.o:$(common)


boot.o:boot.s
	i686-elf-as -o boot.o boot.s

clean:
	rm -f *.o a.out

run:a.out
	qemu-system-i386 -kernel a.out

../common/%.o:../common/%.c ../common/%.h
	make --directory=../common

../common/interrupt.o:../common/interrupt.s
	make --directory=../common

../common/gdt_fush.o:../common/interrupt.s
	make --directory=../common


../video/vga_driver.o:../video/vga_driver.c
	make --directory=../video

%.o:%.cpp
	i686-elf-g++ -o $@ -c $< -ffreestanding  -O2 -Wall -Wextra -fno-exceptions -fno-rtti