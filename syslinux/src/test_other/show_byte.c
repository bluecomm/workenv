#include <stdio.h>

void show_byte(unsigned char *start,int len)
{
	int i;
	for(i=0;i<len;i++)
		printf(" %.2x",start[i]);
		printf("\n");
}

void main()
{
	int a = 0x12345678;
	short int v = -12345;

	show_byte((unsigned char *)&a,sizeof(int));

	unsigned short uv = (unsigned short )v;
	printf("v = %d    uv = %u\n",v,uv);
}

