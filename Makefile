CC = gcc
CFLAGS = -Wall -g -O2 -std=c99 -march=native -mtune=generic
LIBS = -lm -lpng -fopenmp

main: src/*.c
	$(CC) $(CFLAGS) $(LIBS) -o main src/*.c