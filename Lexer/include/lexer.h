#ifndef _LEXER_H
#define _LEXER_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
	T_EOF = 0,
	// 1..255 -> reserved for single chars
	T_NAME = 256,
	T_STRING,
	T_NUMBER,

	T_PLUSEQ,	// +=
	T_MINUSEQ,	// -=
	T_ANDEQ,	// &=
	T_OREQ,		// |=
	
	T_INC,		// ++
	T_DEC,		// --
	T_LSHIFT,	// <<
	T_RSHIFT,	// >>

	T_EQ,		// ==
	T_NEQ,		// !=
	T_LEQ,		// <=
	T_GEQ,		// >=
	T_AND,		// &&
	T_OR,		// ||

	T_TRUE,
	T_FALSE,
	T_DO,
	T_WHILE,
	T_BREAK,
	T_IF,
	T_ELSE,
	T_FOR,
	T_IN,
	T_FUNCTION,
	T_RETURN
} LexTokenKind;

typedef enum {
	LEX_STATE_OK,
	LEX_STATE_ERROR,
	LEX_STATE_EOF
} LexState;

typedef struct {
	char *str;
	size_t length;
	size_t pos;

	size_t tokenStart;
	size_t tokenEnd;

	char currChar;
	char nextChar;

	LexTokenKind token;

	size_t line;		// current token line
	size_t col;			// current token position in line
} LexerState;

LexState lexNextToken(LexerState *);
LexerState * lexNewFromString(char *, unsigned long);
const char * lexTokenToString(TokenKind);

#endif