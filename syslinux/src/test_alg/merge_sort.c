#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_array(int A[], int start, int end){
	int i = 0;
	for (i = start; i <= end; i++)
		printf("%-3d ", A[i]);
	printf("\n");
}


void merge(int A[], int start, int mid, int end){
	int *A1 = (int *)malloc((mid-start+1+1)*sizeof(int));
	int *A2 = (int *)malloc((end-mid+1)*sizeof(int));
	memset(A1, 0, (mid-start+1)*sizeof(int));
	memset(A2, 0, (end-mid)*sizeof(int));

	if (A1 == NULL || A2 == NULL){
		printf("error");
		return;
	}

	int i, j, k;

	for (i = 0; i < mid-start+1; i++ )
		A1[i] = A[start+i];
	A1[i] = 100000;

	for (j = 0; j < end-mid; j++ )
		A2[j] = A[mid+1+j];
	A2[j] = 100000;

	i=0;
	j=0;

	for (k = start; k <= end; k++)
	{
		if (A1[i] <= A2[j])
		{
			A[k] = A1[i];
			i++;
		} else
		{
			A[k] = A2[j];
			j++;
		}
	}

	free(A1);
	free(A2);
}

void merge_sort(int A[], int start, int end){
	if (start >= end)
		return;

	int mid = (end+start)/2;
	merge_sort(A, start, mid);
	merge_sort(A, mid+1, end);
	merge(A, start, mid, end);
}

int main(){
	int A[] = {12,2,4,7,8,1,10,15,3,0,6,9,11,5,13,30,21,33};
	int len = sizeof(A)/sizeof(A[0]);
	int indx = 0;

	for (indx = 0; indx < len; indx++){
		printf("%-3d ", A[indx]);
	}
	printf("\n");

	merge_sort(A, 0, len-1);

	for (indx = 0; indx < len; indx++){
		printf("%-3d ", A[indx]);
	}

	printf("\n");
	return 0;
}
