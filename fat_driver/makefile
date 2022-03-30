all:fat_driver.o main.o makefile
	g++ -g fat_driver.o main.o ../video/vga_driver.o ../ata_driver/ata_driver.o ../common/common.o

clean:
	rm -f fat_driver.o main.o a.out

run:all
	./a.out

%.o:%.cpp
	g++ -g -o $@ -c $<
