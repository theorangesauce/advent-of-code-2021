CC = g++
CFLAGS = -Wall -pedantic

all: day12

day12: main.o
	$(CC) $^ -o day12

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day12
