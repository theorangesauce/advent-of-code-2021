CC = g++
CFLAGS = -Wall -pedantic

all: day16

day16: main.o
	$(CC) $^ -o day16

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day16
