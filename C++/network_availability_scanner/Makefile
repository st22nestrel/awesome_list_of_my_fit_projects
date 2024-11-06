# IPK second project - variant Delta
# Timotej Ponek, xponek00

CC = g++
CFLAGS = -std=c++11 -g -pedantic
PROJ = ipk-l2l3-scan

CPP = ipk-l2l3-scan.cpp args.cpp hosts.cpp IPs.cpp timer.cpp
HEADERS = args.h hosts.h IPs.h timer.h

all:
	$(CC) $(HEADERS) $(CFLAGS) -o $(PROJ) $(CPP)

clean:
	rm -f *.o ipk-l2l3-scan