CC = g++
CFLAGS = -Wall -pedantic

all: day15

day15: main.o
	$(CC) $^ -o day15

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day15
