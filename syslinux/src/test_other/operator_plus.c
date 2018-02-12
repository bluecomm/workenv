#include<stdio.h>

int main(){
	int a = 4;
	a += a++;
	printf("a = %d\n", a);

	a = 4;
	a += ++a;
	printf("a = %d\n", a);

	a = 4;
	++a += a;
	printf("a = %d\n", a);

	a = 4;
	++a += a++;
	printf("a = %d\n", a);

	a = 4;
	++a += ++a;
	printf("a = %d\n", a);

	return 0;
}
