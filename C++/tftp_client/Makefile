# ISA project - TFTP client
# Timotej Ponek, xponek00

CC = g++
CFLAGS = -std=c++11 -g -pedantic
PROJ = tftp-client

CPP = main.cpp args.cpp tftp.cpp utils.cpp datablocks.cpp

all:
	$(CC) $(CFLAGS) -o $(PROJ) $(CPP)

clean:
	rm -f *.o tftp-client