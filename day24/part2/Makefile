CC = g++
CFLAGS = -Wall -pedantic

all: day24

day24: main.o
	$(CC) $^ -o day24

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day24
