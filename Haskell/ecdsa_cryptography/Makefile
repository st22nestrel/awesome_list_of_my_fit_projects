# FLP project1 - ECDSA
# Timotej Ponek, xponek00

CC = ghc
CFLAGS = -Wall -package parsec
SRC = src/Main.hs src/DataOps.hs src/DataTypes.hs src/Parser.hs
OUTPUT = flp22-fun

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)
	rm -f src/*.o src/*.hi

clean:
	rm -f $(OUTPUT)
