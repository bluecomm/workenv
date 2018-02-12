#include <stdio.h>

//int main(){
//	int a[5] = {1,2,3,4,5};
//	int b[100];
//
//	int *ptr = (int *)(&a+1);
//	printf("%d\n%d\n", *(a+1), *(ptr-1));
//	printf("sizeof(b) = %d\n", sizeof(b));
//	printf("sizeof(&b) = %d\n", sizeof(&b));
//
//	return 0;
//}

int main(){
	int a[5] = {1,2,3,4,5};
	int *p = &a[4];
	int i = 0;
	printf("%x\n", a);
	for (i = -4; i <= 0; i++){
		printf("%d %x\n", p[i], &p[i]);
	}
	return 0;
}
