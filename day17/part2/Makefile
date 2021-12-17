CC = g++
CFLAGS = -Wall -pedantic

all: day17

day17: main.o
	$(CC) $^ -o day17

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day17
