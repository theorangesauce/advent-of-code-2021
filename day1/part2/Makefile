CC = g++
CFLAGS = -Wall -pedantic

all: day1

day1: main.o
	$(CC) $^ -o day1

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day1
