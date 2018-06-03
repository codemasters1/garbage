/*
	Generator numerów PESEL by K13
	http://forum.bezpieka.org/User-K13
	01.08.2013
	Wersja 0.0.4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long pesel_t;

pesel_t check_digit(pesel_t pesel)
{
    pesel_t full = pesel;
    const char factors[] = {3, 1, 9, 7};
    char current_digit = 0;
    int tmp = 0;

    for( ; current_digit < 10; current_digit++)
    {
        tmp += (pesel % 10) * factors[current_digit % 4];
        pesel /= 10;
    }

    tmp %= 10;

    if (tmp > 0)
    {
        tmp = 10 - tmp;
    }

    return (full * 10) + tmp;
}

void print_pesel(pesel_t p)
{
    unsigned char out[12]; // 11 digits + \0
    int i = 0;

    memset(out, 0, sizeof(out));

    for ( ; i < 11; i++)
    {
        out[10 - i] = '0' + p % 10;
        p /= 10;
    }

    puts((const char*) out);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("Generator PESEL by K13 v0.0.4\n");
        puts("Uzycie: pesel ROK > plik_docelowy");

        return 2;
    }

    const unsigned char days[]  = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned int year           = 0;
    unsigned int year_max       = 0;
    unsigned char month;
    unsigned char day;
    unsigned char day_max;
    unsigned int i             = 0;


    year = atoi(argv[1]);

    if (year >= 1800 && year <= 2299)
    {
        year_max = year;
    }
    else
    {
        puts("error: nieprawidlowy rok");
        return 3;
    }

    unsigned char month_plus = 0;

    for ( ; year <= year_max; year++)
    {
		if (year >= 1800 && year <= 1899)
            month_plus = 80;
        else if (year >= 2000 && year <= 2099)
            month_plus = 20;
        else if (year >= 2100 && year <= 2199)
            month_plus = 40;
        else if (year >= 2200 && year <= 2299)
            month_plus = 60;
        else
            month_plus = 0;

        for (month = 1; month <= 12; month++)
        {
            day_max = days[month - 1];

			// czy rok jest przestêpny
            if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
            {
                day_max++;
            }

            for (day = 1; day <= day_max; day++)
            {
                for (i = 0; i < 10000; i++)
                {
                    print_pesel(check_digit((year % 100) * (pesel_t) 100000000 + (month + month_plus) * 1000000 + day * 10000 + i));
                }
            }
        }
    }

    return 0;
}
