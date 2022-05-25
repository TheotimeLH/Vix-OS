all:minishell ../kernel/disque
	rm -rf disque_mnt
	mkdir disque_mnt
	sudo mount ../kernel/disque disque_mnt
	sudo cp minishell disque_mnt/mshell
	sudo umount ../kernel/disque
	rm -rf disque_mnt

../kernel/disque:
	cd ../kernel && make disque

%.o: %.c 
	i686-elf-gcc -c $< -o $@ -nostdlib -ffreestanding

minishell: minishell.o ../stdlib/stdlib.o ../stdlib/crt0.o
	i686-elf-gcc $^ -o $@ -nostdlib -ffreestanding

clean :
	rm -f *.o
	rm -f minishell

unmount :
	sudo umount ../kernel/disque

mount: 
	rm -rf disque_mnt
	mkdir disque_mnt
	sudo mount ../kernel/disque disque_mnt
