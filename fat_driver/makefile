all:fat_driver.o main.o makefile
	g++ -g fat_driver.o main.o

clean:
	rm -f fat_driver.o main.o a.out

run:all
	./a.out

%.o:%.cpp
	g++ -g -o $@ -c $<
