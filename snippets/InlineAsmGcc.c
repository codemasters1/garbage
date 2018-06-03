#include <stdio.h>
#include <stdlib.h>

int suma(int, int);
asm(".globl _suma\n\
    _suma:\n\
    xor eax, eax\n\
    mov eax, [esp+8]\n\
    add eax, [esp+4]\n\
    ret 8\n");

int main()
{

	printf("%d\n", suma(1,2));
	printf("%d", suma(111,222));


    return 0;
}
