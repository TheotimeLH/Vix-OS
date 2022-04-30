all:mini_vim ../kernel/disque
	rm -rf disque_mnt
	mkdir disque_mnt
	sudo mount ../kernel/disque disque_mnt
	sudo cp mini_vim disque_mnt/
	sudo umount ../kernel/disque
	rm -rf disque_mnt

../kernel/disque:
	cd ../kernel && make disque

mini_vim:main.c
	i686-elf-gcc -c main.c -nostdlib -ffreestanding
	i686-elf-gcc -o mini_vim main.o ../stdlib/stdlib.o ../stdlib/crt0.o -nostdlib -ffreestanding
