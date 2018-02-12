#include <stdio.h>

void swap_value(int *a,int *b)
{
	*b = *a^*b;
	*a = *a^*b;
	*b = *a^*b;
}

int test_swap(int argc,char **argv)
{
	int first,last,i;
	int a[] = {1,2,3,4,5};
	int b[] = {1,2,3,4,5,6};
	
	printf("a[] = ");
	for(i=0;i<5;i++)
	printf("%d ",a[i]);
	printf("\n");
	
	printf("b[] = ");
	for(i=0;i<6;i++)
	printf("%d ",b[i]);
	printf("\n");
	
	for(first = 0,last = 4;first<=last;first++,last--)
		swap_value(&a[first],&a[last]);
		
	for(first = 0,last = 5;first<=last;first++,last--)
		swap_value(&b[first],&b[last]);

	printf("a[] = ");
	for(i=0;i<5;i++)
	printf("%d ",a[i]);
	printf("\n");
	
	printf("b[] = ");
	for(i=0;i<6;i++)
	printf("%d ",b[i]);
	printf("\n");

	return 0;

}
