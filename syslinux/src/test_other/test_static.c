#include <stdio.h>


/*****************************************************************************/
//static int j;
//
//void fun1(void)
//{
//	static int i = 0;
//	i ++;
//	printf("fun1: i = %d\n", i);
//}
//
//void fun2(void)
//{
//	j = 0;
//	j++;
//	printf("fun2: j = %d\n", j);
//}
//
//int main()
//{
//	int k;
//
//	for(k=0; k<5; k++)
//	{
//		fun1();
//		fun2();
//	}
//
//	return 0;
//}
/*****************************************************************************/

#include<stdio.h>

int my(const int a){
	static int count = 4;
	//count = a;
	count = 5;
	return count + a;
}

int main(){
	printf("%d %d %d\n", my(4), my(5), my(6));
	return 0;
}



