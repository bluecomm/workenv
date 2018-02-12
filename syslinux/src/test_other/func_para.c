#include <stdio.h>

int add(int num,...){
	int sum = 0;
	int indx = 0;
	int *p = NULL;

	p = &num ;

	for (;indx < num ; indx++){
		sum += *p++;
	}
	return sum;
}
int main(){
//	int i = 1;
//	int j = 2;
//	int k = 3;
	printf("%d\n", add(4,1,2,3,4,5));
	return 0;
}
