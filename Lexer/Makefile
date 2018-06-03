CC=gcc
CFLAGS=-c -Wall -Wextra -std=c99 -I ./include
SRC=src

BIN=bin

all: lexer.o utils.o lexer_test.o
	$(CC) -o bin/lexer_test.exe bin/lexer_test.o bin/lexer.o bin/utils.o

run: all
	bin/lexer_test.exe nums_valid.mpp

lexer.o:
	$(CC) $(CFLAGS) -o $(BIN)/lexer.o $(SRC)/lexer.c

utils.o:
	$(CC) $(CFLAGS) -o $(BIN)/utils.o $(SRC)/utils.c

lexer_test.o:
	$(CC) $(CFLAGS) -o $(BIN)/lexer_test.o $(SRC)/lexer_test.c

clean:
	rm -f bin/*