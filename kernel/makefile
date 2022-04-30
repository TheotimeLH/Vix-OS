srcs = $(wildcard ./*.cpp)
objs = $(srcs:.cpp=.o)


compile : $(objs) disque


clean:
	rm -f *.o a.out
	rm -f disque

%.o:%.cpp
	i686-elf-g++ -ggdb -o $@ -c $< -ffreestanding  -Wall -Wextra -fno-exceptions -fno-rtti

disque:prog_test/prog.s prog_test/main.c
	dd if=/dev/zero of=disque count=100
	mkfs.fat disque
	make --directory=prog_test
	sudo rm -rf disque_mnt
	sudo mkdir disque_mnt
	sudo mount disque disque_mnt
	sudo cp prog_test/prog disque_mnt
	sudo umount disque
	sudo rm -r disque_mnt


.PHONY : compile clean
