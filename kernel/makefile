srcs = $(wildcard ./*.cpp)
objs = $(srcs:.cpp=.o)


compile : $(objs)


clean:
	rm -f *.o a.out

%.o:%.cpp
	i686-elf-g++ -ggdb -o $@ -c $< -ffreestanding  -Wall -Wextra -fno-exceptions -fno-rtti

prog:
	make --directory=prog_test
	sudo mount test /mnt/test
	sudo cp prog_test/prog /mnt/test
	sudo umount test


.PHONY : compile clean
