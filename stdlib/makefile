all:crt0.o stdlib.o

crt0.o:crtc.o crts.o
	i686-elf-ld -relocatable crtc.o crts.o -o crt0.o

crtc.o:crt0.c
	i686-elf-gcc -c crt0.c -o crtc.o -ffreestanding -nostdlib

crts.o:crt0.s
	i686-elf-as -c crt0.s -o crts.o

stdlib.o:stdlib.h stdlib.c
	i686-elf-gcc -c stdlib.c -nostdlib -ffreestanding

clean:
	rm -f *.o

.PHONY : all
