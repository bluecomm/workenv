#include<stdio.h>
#include<string.h>

int main(){
	printf("strlen = %d\n", strlen("\0"));
	printf("sizeof = %d\n", sizeof("\0"));
	printf("sizeof = %d\n", sizeof("a\0"));
	printf("sizeof = %d\n", sizeof("aa\0"));
	return 0;
}
