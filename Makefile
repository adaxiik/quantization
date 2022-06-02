CC = gcc
CFLAGS = -Wall -Ofast -std=c99 -march=native -mtune=generic
LIBS = -lm -lpng -fopenmp

main: src/*.c
	$(CC) $(CFLAGS) $(LIBS) -o main src/*.c