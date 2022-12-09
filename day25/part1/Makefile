CC = g++
CFLAGS = -Wall -pedantic

all: day25

day25: main.o
	$(CC) $^ -o day25

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day25
