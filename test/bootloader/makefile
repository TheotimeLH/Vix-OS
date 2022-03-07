all:myos.iso

myos.iso:bootloader.bin
	dd if=bootloader.bin of=myos.iso

bootloader.bin:bootloader.o linker.ld
	i686-elf-ld --oformat binary -o bootloader.bin -T linker.ld bootloader.o

bootloader.o:bootloader.s
	i686-elf-as bootloader.s -o bootloader.o

run:all
	qemu-system-i386 -hda myos.iso

clean:
	rm bootloader.bin bootloader.o myos.iso
