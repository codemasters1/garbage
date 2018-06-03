#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		puts("error: expected 2 arguments");
		return -1;
	}

	FILE *File;
	size_t FileSize = 0;
	char *FileBuffer;

	File = fopen(argv[1], "r");

	if (! File)
	{
		puts("error: could not open the file!");
		return -2;
	}

	fseek(File, 0, SEEK_END);
	FileSize = ftell(File);
	fseek(File, 0, SEEK_SET);

	FileBuffer = malloc(FileSize + 1);
	memset(FileBuffer, 0, FileSize + 1);
	fread(FileBuffer, 1, FileSize, File);

	fclose(File);

	LexerState * lex = lexNewFromString(FileBuffer, FileSize);

	do
	{
		if (lex->token == T_STRING || lex->token == T_NUMBER || lex->token == T_NAME)
		{
			size_t length = lex->tokenEnd - lex->tokenStart;
			char *buf = malloc(length+1);
			buf[length] = '\0';
			strncpy(buf, lex->str + lex->tokenStart, length);
			printf("%s ", buf);
		}
		else
		{
			printf("%s ", lexTokenToString(lex->token));
		}
		

	} while (lexNextToken(lex) == LEX_STATE_OK);

	return 0;
}
