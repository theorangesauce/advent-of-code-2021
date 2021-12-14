CC = g++
CFLAGS = -Wall -pedantic

all: day14

day14: main.o
	$(CC) $^ -o day14

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day14
