CC=g++
CFLAGS=-Wall -Wpedantic -Wextra -std=c++11 -O2

randommusic: randommusic.o
	$(CC) -o randommusic randommusic.o $(CFLAGS)

randommusic.o: randommusic.cc
	$(CC) -c randommusic.cc $(CFLAGS)

clean:
	rm -f *.o
