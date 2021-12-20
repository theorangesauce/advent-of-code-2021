CC = g++
CFLAGS = -Wall -pedantic

all: day20

day20: main.o
	$(CC) $^ -o day20

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day20
