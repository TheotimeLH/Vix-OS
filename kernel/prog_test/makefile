prog:prog.s main.c
	i686-elf-as -o prog.o prog.s
	i686-elf-gcc -c main.c
	i686-elf-gcc -o prog prog.o main.o -nostdlib

clean:
	rm -f prog prog.o main.o

.PHONY : clean
