#include <stdio.h>

void main()
{
	int a = 0x22;
	int b = 0x44;
	int c = 0x1;

	printf("a=0x%x b=0x%x\n",a,b);
	printf("a & b = 0x%x\n",a&b);
	printf("a | b = 0x%x\n",a|b);
	printf("a && b = 0x%x\n",a&&b);
	printf("a || b = 0x%x\n",a||b);
	printf("~a = 0x%x\n",~a);
	printf("!a = 0x%x\n",!a);

	printf("c = %x , c << 33 = %x\n",c,c<<33);

}
