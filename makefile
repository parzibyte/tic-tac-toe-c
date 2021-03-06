CC=gcc
CFLAGS=-Wall
main: main.c
	$(CC) $(CFLAGS) main.c -o main 
	./main