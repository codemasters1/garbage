#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"

bool parseNumber(const char *s, double *out)
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
			case 'x': case 'X':
				format = hexadecimal;
				ptr += 2;

				if (*ptr == '\0')
				{
					return false;
				}

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
					return false;
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
				if (isDecimalDigit(*ptr))
				{
					if (! isOctalDigit(*ptr))
					{
						format = decimal;
					}

					result = result * format + *ptr - '0';
				}
				else
				{
					return false;
				}
				break;
			case decimal:
				switch (*ptr)
				{
					case '.':
						if (floating || exponential)
						{
							return false;
						}
						else
						{
							floating = true;
						}
						break;
					case 'e': case 'E':
						if (exponential)
						{
							return false;
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
							return false;
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
							return false;
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
					return false;
				}
		}
	}

	if (negativeExponent)
		exponent *= -1;

	if (! exponential)
		result += fraction;

	result = result * pow(10, exponent);
	*out = result;

	return true;
}

int main(int argc, char **argv)
{
	if (argc>1)
	{
		double out;
		if (parseNumber(argv[1], &out))
		{
			printf("'%s' = %.15g\n", argv[1], out);
		}
		else
		{
			printf("Invalid format: %s\n", argv[1]);
		}
	}

	return 0;
}