CC = g++
CFLAGS = -Wall -pedantic

all: day2

day2: main.o
	$(CC) $^ -o day2

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day2
