#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

inline bool isDecimalDigit(char c)
{
	return (c >= '0') && (c <= '9');
}

inline bool isHexadecDigit(char c)
{
	return isDecimalDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

inline bool isOctalDigit(char c)
{
	return (c >= '0') && (c <= '7');
}


inline char parseHexDigit(char c)
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

double parseNumber(const char *s)
{
	enum { octal = 8, decimal = 10, hexadecimal = 16 } format = decimal;
	char * ptr = (char*) s;
	bool exponential = false;
	bool floating = false;
	double result = 0;

	if (*ptr == '0')
	{
		char next = *(s + 1);

		switch (next)
		{
			case 'x':
				format = hexadecimal;
				ptr += 2;
				break;
			case '.':
			case 'e': case 'E':
			case '\0':
				break;
			default:
				if (isDecimalDigit(next))
				{
					format = octal;
				}
				else
				{
					puts("error ..! ..! ..!");
				}
		}
	}

	bool expectExponent = false;
	bool expectExponentSign = false;
	bool negativeExponent = false;
	int exponent = 0;
	unsigned int fractionLength = 0;
	double fraction = 0;

	for (; *ptr; ptr++)
	{
		switch (format)
		{
			case octal:
				if (isOctalDigit(*ptr))
				{
					result = result * format + *ptr - '0';
				}
				else
				{
					puts("no i chuj...");
				}
				break;
			case decimal:
				switch (*ptr)
				{
					case '.':
						floating = true;
						break;
					case 'e': case 'E':
						if (exponential)
						{
							puts("errrrrrrrrrrrrrrrrrr");
						}
						else
						{
							exponential = true;
							expectExponentSign = true;
							result += fraction;
						}
						break;
					case '-': case '+':
						if (expectExponentSign)
						{
							negativeExponent = (*ptr == '-');
							expectExponent = true;
						}
						else
						{
							puts("errorrorroorror");
						}
						break;
					default:
						if (isDecimalDigit(*ptr))
						{
							if (expectExponentSign)
							{
								expectExponentSign = false;
								expectExponent = true;
							}

							if (expectExponent)
							{
								exponent = exponent * 10 + *ptr - '0';
							}
							else if (floating)
							{
								fraction += ((double)(*ptr - '0')) / pow(10, ++fractionLength);
							}
							else
							{
								result = result * format + *ptr - '0';
							}
							
						}
						else
						{
							puts("errorrorroorrodf");
						}
						
				}

				break;
			case hexadecimal:
				if (isHexadecDigit(*ptr))
				{
					result = result * format + parseHexDigit(*ptr);
				}
				else
				{
					puts("aaa");
				}
		}
	}

	if (negativeExponent)
		exponent = -exponent;

	if (! exponential)
		result += fraction;

	result = result * pow(10, exponent);

	printf("'%s' = %.15g\n", s, result);

	return result;
}

int main(int argc, char **argv)
{
	if (argc>1)
	{
		parseNumber(argv[1]);
	}

	return 0;
}