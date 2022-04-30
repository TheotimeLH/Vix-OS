prog:main.c
	i686-elf-gcc -c main.c
	i686-elf-gcc -o prog main.o ../../stdlib/stdlib.o ../../stdlib/crt0.o -nostdlib

clean:
	rm -f prog prog.o main.o

.PHONY : clean
