#On fait ici un makefile qui va tout compiler et lancer le noyau comme il est \
	pour l'instant tout se trouve dans test_entry
COMMON_INCLUDE_SRC = $(wildcard common/*.c) $(wildcard common/*.s) $(wildcard common/*.cpp)
COMMON_INCLUDE = $(COMMON_INCLUDE_SRC:.c=.o) #$(COMMON_INCLUDE_SRC:.c=.o) $(COMMON_INCLUDE_SRC:.cpp=.o)
KERNEL_INCLUDE_SRC = $(wildcard kernel/*.c) $(wildcard kernel/*.s) $(wildcard kernel/*.cpp)
KERNEL_INCLUDE = $(KERNEL_INCLUDE_SRC:.s=.o) $(KERNEL_INCLUDE_SRC:.c=.o) $(KERNEL_INCLUDE_SRC:.cpp=.o)
dirs = ./ kernel ata_driver fat_driver common video

find_c_src = $(wildcard $(dir)/*.c)
find_cpp_src = $(wildcard $(dir)/*.cpp)
find_s_src = $(wildcard $(dir)/*.s)
INCLUDE_C = $(foreach dir, $(dirs), $(find_c_src))
INCLUDE_CPP = $(foreach dir, $(dirs), $(find_cpp_src))
INCLUDE_S = $(foreach dir, $(dirs), $(find_s_src))

INCLUDES = $(INCLUDE_C:.c=.o) $(INCLUDE_CPP:.cpp=.o) $(INCLUDE_S:.s=.o)#$(COMMON_INCLUDE) $(KERNEL_INCLUDE) ata_driver/ata_driver.o fat_driver/fat_driver.o video/vga_driver.o
all : run

get_includes :
	@echo "$(INCLUDES)"
common :  # On appeler le makefile de common
	cd common && make

ata_driver : 
	cd ata_driver && make

fat_driver :
	cd fat_driver && make 

video :
	cd video && make

kernel : 
	cd kernel && make

boot.o : boot.s
	i686-elf-as -ggdb --32 $< -o $@

run : VixOS.iso
	qemu-system-i386 -soundhw pcspk -m 2G -drive file=kernel/disque,format=raw -kernel VixOS.bin #-cdrom VixOS.iso 


debug : VixOS.iso
	qemu-system-i386 -s -S -cdrom VixOS.iso

VixOS.bin : linker.ld  $(INCLUDES)
	i686-elf-g++ -ggdb -T $^ -o $@ -ffreestanding -nostdlib
	grub-file --is-x86-multiboot VixOS.bin

VixOS.iso : common video kernel ata_driver fat_driver VixOS.bin grub.cfg 
	mkdir -p isodir/boot/grub
	cp VixOS.bin isodir/boot/VixOS.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o VixOS.iso isodir

clean_all :
	rm -f *.o *.iso
	rm -fr isodir
	$(foreach dir,$(dirs), cd $(dir) && make clean; ) 

clean :
	rm -f *.o *.iso
	rm -fr isodir
	cd common && make clean
	cd fat_driver && make clean
	cd ata_driver && make clean
	cd video && make clean

.PHONY : all run clean common ata_driver fat_driver video kernel
