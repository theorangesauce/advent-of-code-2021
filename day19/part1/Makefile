CC = g++
CFLAGS = -Wall -pedantic

all: day19

day19: main.o
	$(CC) $^ -o day19

main.o: main.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o *~ day19
