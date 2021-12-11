CC = g++
CFLAGS = -Wall -pedantic

all: day11

day11: main.o
	$(CC) $^ -o day11

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day11
