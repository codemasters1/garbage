#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool isWhitespace(char c)
{
	return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDecimalDigit(char c)
{
	return (c >= '0') && (c <= '9');
}

bool isHexadecDigit(char c)
{
	return isDecimalDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool isOctalDigit(char c)
{
	return (c >= '0') && (c <= '7');
}

bool isAlnum(char c)
{
	return isDecimalDigit(c) || isAlpha(c);
}

bool isValidNameChar(char c)
{
	return isAlnum(c) || c == '_' || c == '$';
}

char parseHexDigit(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	
	switch(c)
	{
		case 'a': case 'A': return 0x0A;
		case 'b': case 'B': return 0x0B;
		case 'c': case 'C': return 0x0C;
		case 'd': case 'D': return 0x0D;
		case 'e': case 'E': return 0x0E;
		case 'f': case 'F': return 0x0F;
		default: 			return 0xFF;
	}
}
