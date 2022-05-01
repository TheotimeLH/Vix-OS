srcs = $(wildcard ./*.cpp)
objs = $(srcs:.cpp=.o)


compile : $(objs) disque


clean:
	rm -f *.o a.out
	rm -f disque
	cd prog_test && make clean

%.o:%.cpp
	i686-elf-g++ -o $@ -c $< -ffreestanding  -Wall -Wextra -fno-exceptions -fno-rtti

disque:prog_test/prog.s prog_test/main.c
	dd if=/dev/zero of=disque count=100
	mkfs.fat disque
	make --directory=prog_test
	sudo rm -rf disque_mnt
	sudo mkdir disque_mnt
	sudo mount disque disque_mnt
	sudo cp prog_test/prog disque_mnt
	sudo echo "fichier test">file
	sudo cp file disque_mnt
	sudo rm -f file
	sudo umount disque
	sudo rm -rf disque_mnt


.PHONY : compile clean
