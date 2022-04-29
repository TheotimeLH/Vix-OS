SRC_C = $(wildcard *.c)
OBJ_C = $(SRC_C:.c=.o)
SRC_S = $(wildcard *.s)
OBJ_S = $(SRC_S:.s=.o)

all : $(OBJ_S) $(OBJ_C)


%.o : %.c 
	i686-elf-gcc -g -m32 -c $< -o $@ -ffreestanding -O2 -Wall -Wextra

%.o : %.s
	i686-elf-as -g --32 $< -o $@

clean : 
	rm *.o

.PHONY : clean
