CC = g++
CFLAGS = -Wall -pedantic

all: day9

day9: main.o
	$(CC) $^ -o day9

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day9
