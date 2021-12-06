CC = g++
CFLAGS = -Wall -pedantic

all: day6

day6: main.o
	$(CC) $^ -o day6

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day6
