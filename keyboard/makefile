keyboard.o : keyboard.c keyboard.h
	i686-elf-gcc -g -m32 -c $< -o $@ -ffreestanding -Wall -Wextra

clean : 
	rm -f *.o

.PHONY :
	clean
