CC = g++
CFLAGS = -Wall -pedantic

all: day21

day21: main.o
	$(CC) $^ -o day21

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day21
