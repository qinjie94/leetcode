/*
 * binheap.c
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 */


#include "binheap.h"
#include "../shared/fatal.h"

#define MIN_PQ_SIZE 1

struct HeapStruct {
	int capacity;
	int size;
	ElementType *keys;
	ElementType *values;
};


PriorityQueue initialize_BINHEAP(int MaxElements, const ElementType minKey) {
	PriorityQueue H;

	if (MaxElements < MIN_PQ_SIZE){
		MaxElements = MIN_PQ_SIZE;
	}

	H = malloc( sizeof( struct HeapStruct ) );
	if( H ==NULL )
		fatal_error( "Out of space!!!" );

	/* Allocate the array plus one extra for sentinel */
	H->keys = malloc( ( MaxElements + 1 ) * sizeof( ElementType ) );
	H->values = malloc( ( MaxElements + 1 ) * sizeof( ElementType ) );

	if( H->keys == NULL || H->values == NULL)
		fatal_error( "Out of space!!!" );

	H->capacity = MaxElements;
	H->size = 0;
	H->keys[ 0 ] = minKey;
	H->values[ 0 ] = NULL;
	return H;
}


void make_empty_BINHEAP(PriorityQueue H) {
	H->size = 0;
}


void insert_max_k_BINHEAP(const ElementType Key, const ElementType Value, PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*)) {
	if(is_full_BINHEAP(H)){
		if(compare(find_min_key_BINHEAP(H), Key) < 0){
			free_key(delete_min_BINHEAP(H, compare));
			insert_BINHEAP(Key, Value, H, compare);
		}else{
			free_key(Key);
		}
	}else{
		insert_BINHEAP(Key, Value, H, compare);
	}
}


void insert_BINHEAP(const ElementType Key, const ElementType Value, PriorityQueue H, int (*compare)(const void*, const void*)) {
	int i = 0;

	if (is_full_BINHEAP(H)) {
		fatal_error( "Priority queue is full" );
	}
	for( i = ++H->size; compare(H->keys[ i / 2 ], Key) > 0; i /= 2 ){
		H->keys[ i ] = H->keys[ i / 2 ];
		H->values[ i ] = H->values[ i / 2 ];
	}

	H->keys[ i ] = Key;
	H->values[ i ] = Value;
}


ElementType delete_min_BINHEAP(PriorityQueue H, int (*compare)(const void*, const void*)){
	int i, Child;
	ElementType MinKey, LastKey, LastValue;

	if (is_empty_BINHEAP(H)) {
		error( "Priority queue is empty" );
		return H->keys[ 0 ];
	}

	MinKey = H->keys[ 1 ];
	LastKey = H->keys[ H->size ];
	LastValue = H->values[ H->size-- ];

	for( i = 1; i * 2 <= H->size; i = Child ){
		/* Find smaller child */
		Child = i * 2;
		if( Child != H->size && compare(H->keys[ Child + 1 ], H->keys[ Child ]) < 0 )
			Child++;

		/* Percolate one level */
		if( compare(LastKey, H->keys[ Child ]) > 0 ){
			H->keys[ i ] = H->keys[ Child ];
			H->values[ i ] = H->values[ Child ];
		}else{
			break;
		}
	}
	H->keys[ i ] = LastKey;
	H->values[ i ] = LastValue;
	return MinKey;
}


ElementType find_min_value_BINHEAP(const PriorityQueue H) {
	if (!is_empty_BINHEAP(H))
		return H->values[1];
	error( "Priority Queue is Empty (min_value)" );
	return H->values[ 0 ];
}


ElementType find_min_key_BINHEAP(const PriorityQueue H) {
	if (!is_empty_BINHEAP(H))
		return H->keys[1];
	error( "Priority Queue is Empty (min_key)" );
	return H->keys[ 0 ];
}


static List get_key_value_sorted_list_helper(const PriorityQueue H, int (*compare)(const void*, const void*), int reverseSortOrder, void (*free_key)(void*)){
	List sortedList = make_empty_pointer_LIST( NULL );
	KeyValuePair *currEntry;

	while (!is_empty_BINHEAP(H)){
		currEntry = malloc(sizeof(KeyValuePair));
		currEntry->key = find_min_key_BINHEAP(H);
		currEntry->value = find_min_value_BINHEAP(H);

		if(reverseSortOrder)
			insert_first_LIST(currEntry, sortedList);
		else
			insert_last_LIST(currEntry, sortedList);

		free_key(delete_min_BINHEAP(H, compare));
	}

	return sortedList;
}


List get_key_value_reverse_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*)){
	return get_key_value_sorted_list_helper(H, compare, TRUE, free_key);
}

List get_key_value_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*)){
	return get_key_value_sorted_list_helper(H, compare, FALSE, free_key);
}



List get_value_sorted_list_helper(const PriorityQueue H, int (*compare)(const void*, const void*), int reverseSortOrder, void (*free_key)(void*)){
	List sortedList = make_empty_pointer_LIST( NULL );

	while (!is_empty_BINHEAP(H)){

		if(reverseSortOrder)
			insert_first_LIST(find_min_value_BINHEAP(H), sortedList);
		else
			insert_last_LIST(find_min_value_BINHEAP(H), sortedList);

		free_key(delete_min_BINHEAP(H, compare));
	}
	return sortedList;
}


List get_value_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*)){
	return get_value_sorted_list_helper(H, compare, FALSE, free_key);
}

List get_value_reverse_sorted_list_BINHEAP(const PriorityQueue H, int (*compare)(const void*, const void*), void (*free_key)(void*)){
	return get_value_sorted_list_helper(H, compare, TRUE, free_key);
}


void print_BINHEAP(const PriorityQueue H, char* (*to_string)(const void* x)) {
	int i = 0;
	char* buf;
	char result[MAX_BUFFER_SIZE_FOR_PRINTING];

	strcpy(result, "");
	for( i = 1; i <= H->size; i++ ){
		strcat(result, "(");
		buf = to_string(H->keys[ i ]);
		strcat(result, buf);
		free(buf);
		strcat(result, ",");
		buf = to_string(H->values[ i ]);
		strcat(result, buf);
		strcat(result, "),");
		free(buf);
	}
	puts(result);
}


int is_empty_BINHEAP(const PriorityQueue H) {
	return H->size == 0;
}


int get_size_BINHEAP(const PriorityQueue H) {
	return H->size;
}


int is_full_BINHEAP(const PriorityQueue H) {
	return H->size == H->capacity;
}



PriorityQueue destroy_BINHEAP(PriorityQueue H, void (*free_key)(void*), void (*free_value)(void*)) {
	int i;

	for( i = 1; i <= H->size; i++ ){
		free_key(H->keys[ i ]);
		free_value(H->values[ i ]);
	}

	free_key(H->keys[0]);

	free(H->keys);
	free(H->values);
	free(H);
	return NULL;
}

