/*
 *  binheap.h
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 */



#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../shared/generictype.h"
#include "../linkedlist/list.h"

#ifndef _BinHeap_H
#define _BinHeap_H

#define MAX_QUEUE_SIZE_PRINTING 2000
#define MAX_BUFFER_SIZE_FOR_PRINTING 2000
#define MAX_BUFFER_SIZE_FOR_CONVERSION 10
#define DECIMAL_BASE 10

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;

PriorityQueue initialize_BINHEAP(int MaxElements, const ElementType minKey);
PriorityQueue destroy_BINHEAP(PriorityQueue H, void (*free_key)(void*), void (*free_value)(void*));
void make_empty_BINHEAP(PriorityQueue H);
void insert_BINHEAP(const ElementType Key, const ElementType Value, PriorityQueue H, int (*compare)(const void*, const void*));
void insert_max_k_BINHEAP(const ElementType Key, const ElementType Value, PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*));
ElementType delete_min_BINHEAP(PriorityQueue H, int (*compare)(const void*, const void*));
ElementType find_min_key_BINHEAP(const PriorityQueue H);
ElementType find_min_value_BINHEAP(const PriorityQueue H);
int is_empty_BINHEAP(const PriorityQueue H);
int get_size_BINHEAP(const PriorityQueue H);
int is_full_BINHEAP(const PriorityQueue H);
void print_BINHEAP(const PriorityQueue H, char* (*to_string)(const void* x));
List get_value_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*));
List get_value_reverse_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*));
List get_key_value_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*));
List get_key_value_reverse_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*));

#endif

/* END */

