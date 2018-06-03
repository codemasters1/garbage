all:
	gcc -Wall -Wextra -B . -I ./include -o bin/lexer.exe lexer.c src/lexer.c src/utils.c

run: all
	bin/lexer.exe nums_valid.mpp
	bin/lexer.exe nums_invalid.mpp
