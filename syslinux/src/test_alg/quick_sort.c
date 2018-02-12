#include <stdio.h>

int partition(int A[], int start, int end) {
	if (start == end)
		return start;

	int star = A[end];

	int indx1 = start - 1;
	int indx2;
	int tmp = 0;

	for (indx2 = start; indx2 < end; indx2++) {
		if (A[indx2] <= star) {
			indx1++;
			tmp = A[indx2];
			A[indx2] = A[indx1];
			A[indx1] = tmp;
		}
	}

	tmp = A[indx1 + 1];
	A[indx1 + 1] = star;
	A[end] = tmp;

	return indx1 + 1;
}

void quick_sort(int A[], int start, int end) {
	if (start >= end)
		return;
	int q = partition(A, start, end);
	quick_sort(A, start, q - 1);
	quick_sort(A, q + 1, end);
	return;
}

int main() {
	int A[] = { 12, 2, 4, 7, 8, 1, 10, 15, 3, 0, 6, 9, 11, 5, 13, 30, 21 };
	int len = sizeof(A) / sizeof(A[0]);
	int indx = 0;

	for (indx = 0; indx < len; indx++) {
		printf("%-3d ", A[indx]);
	}
	printf("\n");

	quick_sort(A, 0, len - 1);

	for (indx = 0; indx < len; indx++) {
		printf("%-3d ", A[indx]);
	}

	printf("\n");
	return 0;
}
