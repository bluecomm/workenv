#include <stdio.h>

int main(){
	int tmp = 1;

	const int *p1;
	int const *p2;
	int *const p3 = &tmp;

	int a = 1;
	const int b = 2;

	p1 = &a;
	// assignment of read-only location ‘*p1’
	//*p1 = 2;
	p1 = &b;

	p2 = &a;
	//assignment of read-only location ‘*p2’
	//*p2 = 2;
	p2 = &b;

	printf("tmp = %d\n", tmp);
	*p3 = 2;
	printf("tmp = %d\n", tmp);
	//assignment of read-only variable ‘p3’
	//p3 = &a;

	return 0;
}
