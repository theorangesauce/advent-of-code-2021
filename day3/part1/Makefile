CC = g++
CFLAGS = -Wall -pedantic

all: day3

day3: main.o
	$(CC) $^ -o day3

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day3
