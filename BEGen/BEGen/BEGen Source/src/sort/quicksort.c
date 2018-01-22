/*
 * quicksort.c
 *
 *  Created on: Nov 17, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 */

#include "quicksort.h"

#define Cutoff ( 3 )

void insertion_sort(ElementType A[], int N, int compare(const void*, const void*)) {
	int j, P;
	ElementType Tmp;

	for (P = 1; P < N; P++) {
		Tmp = A[P];
		for (j = P; j > 0 && compare(A[j - 1], Tmp) > 0; j--)
			A[j] = A[j - 1];
		A[j] = Tmp;
	}
}

void swap(ElementType *Lhs, ElementType *Rhs) {
	ElementType Tmp = *Lhs;
	*Lhs = *Rhs;
	*Rhs = Tmp;
}

static ElementType medianThree(ElementType A[], int Left, int Right, int compare(const void*, const void*)) {
	int Center = (Left + Right) / 2;

	if (compare(A[Left], A[Center]) > 0)
		swap(&A[Left], &A[Center]);
	if (compare(A[Left], A[Right]) > 0)
		swap(&A[Left], &A[Right]);
	if (compare(A[Center], A[Right]) > 0)
		swap(&A[Center], &A[Right]);

	/* Invariant: A[ Left ] <= A[ Center ] <= A[ Right ] */

    swap( &A[ Center ], &A[ Right - 1 ] );  /* Hide pivot */
	return A[Right - 1]; /* Return pivot */
}

static void quickSortHelper(ElementType A[], int Left, int Right, int compare(const void*, const void*)) {
	int i, j;
	ElementType Pivot;

	if (Left + Cutoff <= Right) {
		Pivot = medianThree(A, Left, Right, compare);

		i = Left;
		j = Right - 1;

		for (;;) {
			while (compare(A[++i], Pivot) < 0) {
			}
			while (compare(A[--j], Pivot) > 0) {
			}
			if (i < j)
				swap(&A[i], &A[j]);
			else
				break;
		}

		swap(&A[i], &A[Right - 1]); /* Restore pivot */

		quickSortHelper(A, Left, i - 1, compare);
		quickSortHelper(A, i + 1, Right, compare);
	} else
		/* Do an insertion sort on the subarray */
		insertion_sort(A + Left, Right - Left + 1, compare);
}

void quick_sort(ElementType A[], int N, int compare(const void*, const void*)) {
	quickSortHelper(A, 0, N - 1, compare);
}
