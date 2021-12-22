CC = g++
CFLAGS = -Wall -pedantic

all: day22

day22: main.o
	$(CC) $^ -o day22

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day22
