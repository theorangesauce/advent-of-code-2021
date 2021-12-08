CC = g++
CFLAGS = -Wall -pedantic

all: day8

day8: main.o
	$(CC) $^ -o day8

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day8
