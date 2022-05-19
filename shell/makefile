all:shell ../kernel/disque
	rm -rf disque_mnt
	mkdir disque_mnt
	sudo mount ../kernel/disque disque_mnt
	sudo cp shell disque_mnt/
	sudo umount ../kernel/disque
	rm -rf disque_mnt
	cd ../ && make

parser.c parser.h:parser.y
	bison -o parser.c parser.y --header=parser.h

%.o:%.c
	i686-elf-gcc -c $< -ffreestanding -nostdlib

shell:parser.o shell.o ../stdlib/stdlib.o ../common/malloc.o
	i686-elf-gcc -o $@ $^ -nostdlib
