CC = g++
CFLAGS = -Wall -pedantic

all: day13

day13: main.o
	$(CC) $^ -o day13

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day13
