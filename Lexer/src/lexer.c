#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "lexer.h"
#include "utils.h"

static inline bool compareToken(char *str, const char token[])
{
	return ! strncmp(str, token, sizeof(token));
}

static inline void lexNextChar(LexerState *lex)
{
	lex->pos++;

	if (lex->currChar == '\n')
	{
		lex->line++;
		lex->col = 1;
	}
	else
	{
		lex->col++;
	}

	lex->currChar = lex->str[lex->pos];
	lex->nextChar = lex->str[lex->pos + 1];
}

static LexState lexUnexpectedChar(LexerState * lex)
{
	printf("\nUnexpected '%c' at %d:%d.\n", lex->currChar, lex->line, lex->col);
	return LEX_STATE_ERROR;
}

LexState lexNextToken(LexerState *lex)
{
	if (lex->token == T_EOF)
	{
		return LEX_STATE_EOF;
	}

	// skip whitespaces
	while (lex->currChar && isWhitespace(lex->currChar))
		lexNextChar(lex);

	// single line comments
	if (lex->currChar == '/' && lex->nextChar == '/')
	{
		while (lex->currChar && lex->currChar != '\n')
			lexNextChar(lex);

		return lexNextToken(lex);
	}

	// block comments
	if (lex->currChar == '/' && lex->nextChar == '*')
	{
		while (lex->currChar && (lex->currChar != '*' || lex->nextChar != '/'))
			lexNextChar(lex);

		lexNextChar(lex); // skip comment closure
		lexNextChar(lex);

		return lexNextToken(lex);
	}

	//size_t line = lex->currCharLine;
	//size_t col = lex->currCharCol;
	
	// string literals
	if (lex->currChar == '"' || lex->currChar == '\'')
	{
		lex->tokenStart = lex->pos;
		char delim = lex->currChar;
		
		lexNextChar(lex);
		
		while (lex->currChar && lex->currChar != delim)
		{
			if (lex->currChar == '\\' && lex->nextChar == delim)
			{
				lexNextChar(lex);
			}

			lexNextChar(lex);
		}

		lex->tokenEnd = lex->pos + 1;
		lex->token = T_STRING;

		lexNextChar(lex);
	}
	// numbers
	else if (isDecimalDigit(lex->currChar))
	{
		enum {octal, decimal, hexadecimal} format = decimal;
		bool exponential = false;
		bool floating = false;

		lex->tokenStart = lex->pos;

		if (lex->currChar == '0')
		{
			if (lex->nextChar == 'x' || lex->nextChar == 'X')
			{
				format = hexadecimal;

				lexNextChar(lex);
				lexNextChar(lex); // skip 0x
			}
			else if (lex->nextChar == 'e' || lex->nextChar == 'E')
			{
				// allow
			}
			else if (isDecimalDigit(lex->nextChar))
			{
				format = octal;
			}
			else if (isAlpha(lex->nextChar))
			{
				lexUnexpectedChar(lex);
			}
		}

		while (lex->currChar)
		{
			if (lex->currChar == '.')
			{
				if (format != decimal || floating)
					return lexUnexpectedChar(lex);
				
				floating = true;
				lexNextChar(lex);
			}
			else if (! isAlnum(lex->currChar))
			{
				break;
			}
			else if (lex->currChar == 'e' || lex->currChar == 'E')
			{
				if (exponential)
					return lexUnexpectedChar(lex);
				
				exponential = true;
				lexNextChar(lex);

				if (lex->currChar == '-' || lex->currChar == '+')
				{
					lexNextChar(lex);
					lexNextChar(lex);
				}
			}
			else
			{
				if (format == decimal)
				{
					if (! isDecimalDigit(lex->currChar))
						return lexUnexpectedChar(lex);

					lexNextChar(lex);
				}
				else if (format == hexadecimal)
				{
					if (! isHexadecDigit(lex->currChar))
						return lexUnexpectedChar(lex);
					
					lexNextChar(lex);
				}
				else if (format == octal)
				{
					if (! isOctalDigit(lex->currChar))
						return lexUnexpectedChar(lex);
					
					lexNextChar(lex);
				}
			}
		}

		lex->tokenEnd = lex->pos;
		lex->token = T_NUMBER;
	}
	// names
	else if (isValidNameChar(lex->currChar))
	{
		lex->tokenStart = lex->pos;
		while (lex->currChar && isValidNameChar(lex->currChar))
			lexNextChar(lex);
		lex->tokenEnd = lex->pos;

		assert(lex->tokenEnd - lex->tokenStart > 0);

		lex->token = T_NAME;

		char *text = lex->str + lex->tokenStart;

		switch (text[0])
		{
		case 'b':
			if (compareToken(text, "break"))
			{
				lex->token = T_BREAK;
			}

			break;
		case 'd':
			if (compareToken(text, "do"))
			{
				lex->token = T_DO;
			}
			
			break;
		case 'e':
			if (compareToken(text, "else"))
			{
				lex->token = T_ELSE;
			}

			break;
		case 'f':
			if (compareToken(text, "false"))
			{
				lex->token = T_FALSE;
			}
			else if (compareToken(text, "for"))
			{
				lex->token = T_FOR;
			}
			else if (compareToken(text, "function"))
			{
				lex->token = T_FUNCTION;
			}
			
			break;
		case 'i':
			if (compareToken(text, "if"))
			{
				lex->token = T_IF;
			}

			break;
		case 'r':
			if (compareToken(text, "return"))
			{
				lex->token = T_RETURN;
			}

			break;
		case 't':
			if (compareToken(text, "true"))
			{
				lex->token = T_TRUE;
			}

			break;
		case 'w':
			if (compareToken(text, "while"))
			{
				lex->token = T_WHILE;
			}
			
			break;
		}
	}
	// others
	else
	{
		lex->token = lex->currChar;

		if (lex->currChar)
			lexNextChar(lex);

		if (lex->token == '=' && lex->currChar == '=')
		{
			lex->token = T_EQ;
			lexNextChar(lex);
		}
		else if (lex->token == '+' && lex->currChar == '=')
		{
			lex->token = T_PLUSEQ;
			lexNextChar(lex);
		}
		else if (lex->token == '+' && lex->currChar == '+')
		{
			lex->token = T_INC;
			lexNextChar(lex);
		}
		else if (lex->token == '-' && lex->currChar == '=')
		{
			lex->token = T_MINUSEQ;
			lexNextChar(lex);
		}
		else if (lex->token == '-' && lex->currChar == '-')
		{
			lex->token = T_DEC;
			lexNextChar(lex);
		}
		else if (lex->token == '&' && lex->currChar == '&')
		{
			lex->token = T_AND;
			lexNextChar(lex);
		}
		else if (lex->token == '|' && lex->currChar == '|')
		{
			lex->token = T_OR;
			lexNextChar(lex);
		}
		else if (lex->token == '&' && lex->currChar == '=')
		{
			lex->token = T_ANDEQ;
			lexNextChar(lex);
		}
		else if (lex->token == '<' && lex->currChar == '<')
		{
			lex->token = T_LSHIFT;
			lexNextChar(lex);
		}
		else if (lex->token == '<' && lex->currChar == '=')
		{
			lex->token = T_LEQ;
			lexNextChar(lex);
		}
		else if (lex->token == '>' && lex->currChar == '>')
		{
			lex->token = T_RSHIFT;
			lexNextChar(lex);
		}
		else if (lex->token == '>' && lex->currChar == '=')
		{
			lex->token = T_GEQ;
			lexNextChar(lex);
		}
		else if (lex->token == '!' && lex->currChar == '=')
		{
			lex->token = T_NEQ;
			lexNextChar(lex);
		}
	}

	return LEX_STATE_OK;
}

const char * lexTokenToString(LexTokenKind kind)
{
	switch(kind)
	{
		case T_EOF:			return "T_EOF";
		case T_NAME:		return "T_NAME";
		case T_STRING: 		return "T_STRING";
		case T_NUMBER:		return "T_NUMBER";
	
		case T_PLUSEQ:   	return "+=";
		case T_MINUSEQ:  	return "-=";
		
		case T_EQ:  		return "==";
		case T_LEQ:	    	return "<=";
		case T_GEQ:	    	return ">=";
		case T_AND:      	return "&&";
		case T_OR: 	    	return "||";
	
		case T_INC:    		return "++";
		case T_DEC:  		return "--";
		
		case T_BREAK: 	    return "T_BREAK";
		case T_ELSE:        return "T_ELSE";
		case T_FALSE:       return "T_FALSE";
		case T_FOR: 	    return "T_FOR";
		case T_FUNCTION:    return "T_FUNCTION";
		case T_IF:          return "T_IF";
		case T_RETURN:      return "T_RETURN";
		case T_TRUE:        return "T_TRUE";
		case T_WHILE:		return "T_WHILE";
		default:
		{
			char *buff = malloc(5);
			
			if (kind > 0 && kind < 256)
			{
				sprintf(buff, "%c", kind);
			}
			else
			{
				itoa(kind, buff, 4);
			}
			
			return buff;
		}
	}
}


LexerState * lexNewFromString(char *str, unsigned long length)
{
    assert(str != NULL);
    assert(length > 0);

	LexerState *lex = malloc(sizeof(LexerState));

	lex->str = str;
	lex->length = length;

	lex->pos = 0;
	lex->currChar = str[0];
	lex->nextChar = str[1];
	lex->line = 1;
	lex->col = 1;
	//lex->currCharLine = 1;
	//lex->currCharCol = 1;

	lexNextToken(lex);

	return lex;
}