CC = g++
CFLAGS = -Wall -pedantic

all: day5

day5: main.o
	$(CC) $^ -o day5

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day5
