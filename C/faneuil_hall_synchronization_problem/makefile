# makefile

CC=gcc
CFLAGS= -g -std=gnu99 -Wall -Wextra -pedantic -pthread -lrt

all: proj2

proj2: proj2.c
	$(CC) $(CFLAGS) proj2.c -lm -o proj2

clean:
	rm -f *.o proj2 proj2.out