CC = g++
CFLAGS = -Wall -pedantic

all: day7

day7: main.o
	$(CC) $^ -o day7

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day7
