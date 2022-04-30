all:mvim ../kernel/disque
	rm -rf disque_mnt
	mkdir disque_mnt
	sudo mount ../kernel/disque disque_mnt
	sudo cp mvim disque_mnt/
	sudo umount ../kernel/disque
	rm -rf disque_mnt

../kernel/disque:
	cd ../kernel && make disque

mvim:main.c
	i686-elf-gcc -c main.c -nostdlib -ffreestanding
	i686-elf-gcc -o mvim main.o ../stdlib/stdlib.o ../stdlib/crt0.o -nostdlib -ffreestanding

clean :
	rm -f *.o
	rm -f mvim
