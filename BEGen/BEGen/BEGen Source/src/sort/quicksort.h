/*
 * quicksort.h
 *
 *  Created on: Nov 17, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 *
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include "../shared/fatal.h"
#include "../shared/generictype.h"


void quick_sort(ElementType A[], int N, int compare(const void*, const void*));
void insertion_sort(ElementType A[], int N, int compare(const void*, const void*));
void swap(ElementType *Lhs, ElementType *Rhs);

#endif /* QUICKSORT_H_ */
