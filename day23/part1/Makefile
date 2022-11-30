CC = g++
CFLAGS = -Wall -pedantic

all: day23

day23: main.o
	$(CC) $^ -o day23

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day23
