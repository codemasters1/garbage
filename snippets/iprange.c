#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void)
{
	int count = 0;
	scanf("%d", &count);

	while (count--)
	{
		unsigned char ip[4];
		unsigned char mask[4];
		
		scanf("%hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd", 
			&ip[3], &ip[2], &ip[1], &ip[0],
			&mask[3], &mask[2], &mask[1], &mask[0]
		);
		
		unsigned char first[4];
		unsigned char last[4];
		
		memcpy((unsigned int *) first, (unsigned int *) ip, 4);
		memcpy((unsigned int *) last, (unsigned int *) ip, 4);
		
		first[0] &= mask[0];
		first[1] &= mask[1];
		first[2] &= mask[2];
		first[3] &= mask[3];
		
		last[0] |= ~mask[0];
		last[1] |= ~mask[1];
		last[2] |= ~mask[2];
		last[3] |= ~mask[3];
		
		unsigned int hosts = *(unsigned int*) &last[0] - *(unsigned int*) &first[0];
		
		(*((unsigned int*) &first[0]))++;
		(*((unsigned int*) & last[0]))--;
		
		if (hosts <= 2)
		{
			printf("%u\n", 0);
			
		}
		else
		{
			printf("%u\n", hosts - 1);
			printf("%d.%d.%d.%d %d.%d.%d.%d\n", 
				first[3], first[2], first[1], first[0],
				last[3], last[2], last[1], last[0]
			);
		}
	}

	return 0;
}