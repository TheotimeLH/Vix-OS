INCLUDE_C = $(wildcard ./*.c)
INCLUDE_O = $(INCLUDE_C:.c=.o)


all:mvim ../kernel/disque
	rm -rf disque_mnt
	mkdir disque_mnt
	sudo mount ../kernel/disque disque_mnt
	sudo cp mvim disque_mnt/
	sudo umount ../kernel/disque
	rm -rf disque_mnt

../kernel/disque:
	cd ../kernel && make disque

%.o: %.c 
	i686-elf-gcc -c $< -o $@ -nostdlib -ffreestanding

mvim: linker.ld ../stdlib/stdlib.o ../stdlib/crt0.o ../common/malloc.o  $(INCLUDE_O)
	i686-elf-gcc -T $^ -o $@ -nostdlib -ffreestanding

clean :
	rm -f *.o
	rm -f mvim

unmount :
	sudo umount ../kernel/disque

mount: 
	rm -rf disque_mnt
	mkdir disque_mnt
	sudo mount ../kernel/disque disque_mnt

test : all
	make --directory=../

.PHONY: clean test all unmount mount
