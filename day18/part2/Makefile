CC = g++
CFLAGS = -Wall -pedantic

all: day18

day18: main.o
	$(CC) $^ -o day18

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day18
