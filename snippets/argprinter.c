#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i;

    printf("SSAAP -> Small and Simple Application Arguments Printer\n\n");

    for(i = 0; i < argc; i++)
    {
        printf("%d: \"%s\"", i, argv[i]);
    }

    printf("\n\n");
    printf("(c) 2013");

    return 0;
}
