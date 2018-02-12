#include <stdio.h>

int parent(int i){
	return (i-1)/2;
}

int left(int i){
	return (2*i + 1);
}

int right(int i){
	return (2*i+2);
}

void max_heapify(int A[], int indx, int heap_size){
	int l = left(indx);
	int r = right(indx);

	int largest = indx;

	if (l <= heap_size-1 && A[l] > A[indx]){
		largest = l;
	} else {
		largest = indx;
	}

	if (r <= heap_size-1 && A[r] > A[largest]){
		largest = r;
	}

	if (largest != indx){
		int tmp = A[largest];
		A[largest] = A[indx];
		A[indx] = tmp;
		max_heapify(A, largest, heap_size);
	}
}

void build_max_heap(int A[], int heap_size){
	int indx;
	for (indx = heap_size/2; indx >= 0; indx--){
		//printf("indx: %d\n", indx);
		max_heapify(A, indx, heap_size);
	}

	for (indx = 0; indx < heap_size; indx++){
		printf("%-3d ", A[indx]);
	}
	printf("\n");

}


void heap_sort(int A[], int heap_size){

	build_max_heap(A, heap_size);

	int indx = 0;
	int tmp;

	for (indx = heap_size-1; indx > 0; indx--){

		tmp = A[indx];
		A[indx] = A[0];
		A[0] = tmp;

		heap_size--;
		//printf("heap_size: %d\n", heap_size);
		max_heapify(A, 0, heap_size);
	}
}


int main(){
	int A[] = {12,2,4,7,8,1,10,15,3,0,6,9,11,5,13,30,21,33};
	int len = sizeof(A)/sizeof(A[0]);
	int indx = 0;

	for (indx = 0; indx < len; indx++){
		printf("%-3d ", A[indx]);
	}
	printf("\n");

	heap_sort(A, len);

	for (indx = 0; indx < len; indx++){
		printf("%-3d ", A[indx]);
	}

	printf("\n");
	return 0;
}
