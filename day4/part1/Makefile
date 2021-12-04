CC = g++
CFLAGS = -Wall -pedantic

all: day4

day4: main.o
	$(CC) $^ -o day4

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day4
