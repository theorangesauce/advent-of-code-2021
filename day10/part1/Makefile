CC = g++
CFLAGS = -Wall -pedantic

all: day10

day10: main.o
	$(CC) $^ -o day10

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day10
