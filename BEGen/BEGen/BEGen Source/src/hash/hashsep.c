/*
 * hashseq.c
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 */


#include "../shared/commontype.h"
#include "hashsep.h"


#define MinTableSize (10)

struct ListNode {
	ElementType value;
	ElementType key;
	HashPosition next;
};


/* List *TheList will be an array of lists, allocated later */
/* The lists use headers (for simplicity), */
/* though this wastes space */
struct HashTbl {
	int tableSize;
	HashPosition* lists;
};


struct HashIterator {
	int currHashPosition;
	HashTable hashTable;
};


static const uint32_t primes[] = {
7, 13, 19, 29, 37, 53, 97, 193, 389,
769, 1543, 3079, 6151,
12289, 24593, 49157, 98317,
196613, 393241, 786433, 1572869,
3145739, 6291469, 12582917, 25165843,
50331653, 100663319, 201326611, 402653189,
805306457, 1610612741
};

/* Return next prime; assume N >= 10 */
static int NextPrime_HASH(int N) {
	int i;

	if (N % 2 == 0)
		N++;
	for (;; N += 2) {
		for (i = 3; i * i <= N; i += 2)
			if (N % i == 0)
				goto ContOuter;
		return N;
		ContOuter: ;
	}
}

/* Hash function for ints */
Index Hash_HASH(const ElementType Key, int TableSize, uint32_t  (*hash_key)(const void*)) {
	return hash_key(Key) % TableSize;
}

HashTable create_HASH(int TableSize) {
	HashTable H;
	int i;

	if (TableSize < MinTableSize) {
		TableSize = MinTableSize;
	}

	/* Allocate table */
	H = malloc( sizeof( struct HashTbl ) );
	if( H == NULL )
		fatal_error( "Out of space!!!" );

	H->tableSize = NextPrime_HASH( TableSize );

	/* Allocate array of lists */
	H->lists = malloc( sizeof( HashPosition ) * H->tableSize );
	if( H->lists == NULL )
		fatal_error( "Out of space!!!" );

	/* Allocate list headers */
	for( i = 0; i < H->tableSize; i++ ){
		H->lists[ i ] = NULL;
		/*H->lists[ i ] = malloc( sizeof( struct ListNode ) );
		if( H->lists[ i ] == NULL )
			fatal_error( "Out of space!!!" );
		else{
			H->lists[ i ]->next = NULL;
			H->lists[ i ]->key = NULL;
			H->lists[ i ]->value = NULL;
		}*/
	}

	return H;
}

HashPosition find_HASH(const ElementType Key, const HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*)) {
	HashPosition P;
	HashPosition L;

	P = NULL;
	L = H->lists[Hash_HASH(Key, H->tableSize, hash_key)];

	if(L != NULL){
		P = L->next;

		while (P != NULL && compare(P->key, Key) != 0)
			P = P->next;
	}

	return P;
}

/* If Key already exists in the hash, its value will be replaced by the new value */
void insert_HASH(const ElementType Key, const ElementType Element, HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*)) {
	HashPosition NewCell;
	HashPosition L;
	Index index;

	NewCell = malloc(sizeof(struct ListNode));
	if (NewCell == NULL)
		fatal_error( "Out of space!!!" );
	else{
		index = Hash_HASH( Key, H->tableSize, hash_key);

		if(H->lists[ index ] == NULL){
			H->lists[ index ] = malloc( sizeof( struct ListNode ) );

			if( H->lists[ index ] == NULL )
				fatal_error( "Out of space!!!" );
			else{
				H->lists[ index ]->next = NULL;
				H->lists[ index ]->key = NULL;
				H->lists[ index ]->value = NULL;
			}
		}
		L = H->lists[ index ];

		NewCell->next = L->next;
		NewCell->key = Key;
		NewCell->value = Element;
		L->next = NewCell;
		}
}

void remove_HASH(const ElementType Key, HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*), void (*free_key)(void*), void (*free_value)(void*)){
	HashPosition curr;
	HashPosition prev;
	Index index;

	index = Hash_HASH(Key, H->tableSize, hash_key);
	prev = H->lists[index];

	if(prev != NULL){
		curr = prev->next;

		while (curr != NULL && compare(curr->key, Key) != 0){
			prev = curr;
			curr = curr->next;
		}

		if(curr != NULL){
			prev->next = curr->next;
			free_value(curr->value);
			free_key(curr->key);
			free(curr);
		}

		if(H->lists[index]->next == NULL){
			free(H->lists[index]);
			H->lists[index] = NULL;
		}
	}
}

HashTable clone_HASH(HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*), void* (*clone_key)(void*), void* (*clone_value)(void*)){
	HashTable clone = create_HASH(H->tableSize);

	HashPosition P;
	HashIterator iterator = get_iterator_HASH(H);

	ElementType key;
	ElementType value;

	while(iterator_has_next_HASH(iterator)){
		P = iterator_next_HASH(iterator);
		while((P = advance_HASH(P)) != NULL){
			key = clone_key(retrieve_key_HASH(P));
			value = clone_value(retrieve_value_HASH(P));
			insert_HASH(key, value, clone, hash_key, compare);
		}
	}
	free(iterator);

	return clone;
}


HashTable destroy_HASH(HashTable H, void (*free_key)(void*), void (*free_value)(void*)) {
	int i;

	for (i = 0; i < H->tableSize; i++) {
		if(H->lists[i] != NULL){
			HashPosition P = H->lists[i]->next;
			HashPosition Tmp;

			H->lists[i]->next = NULL;
			while (P != NULL) {
				Tmp = P->next;
				free_value(P->value);
				free_key(P->key);
				free(P);
				P = Tmp;
			}
			free(H->lists[i]);
		}
	}

	free(H->lists);
	free(H);
	return NULL;
}


HashIterator get_iterator_HASH(const HashTable H) {
	HashIterator iterator = malloc( sizeof( HashIterator ) );
	iterator->currHashPosition = -1;
	iterator->hashTable = H;
	return iterator;
}

HashPosition iterator_next_HASH(const HashIterator I){
	return I->hashTable->lists[I->currHashPosition];
}


int iterator_has_next_HASH(HashIterator I){
	while(++I->currHashPosition < I->hashTable->tableSize){
		if(I->hashTable->lists[I->currHashPosition] != NULL){
			return TRUE;
		}
	}
	return FALSE;
}


ElementType retrieve_value_HASH(const HashPosition P) {
	if(P == NULL)
		return NULL;
	return P->value;
}

ElementType retrieve_key_HASH(const HashPosition P) {
	if(P == NULL)
		return NULL;
	return P->key;
}

HashPosition advance_HASH(const HashPosition P) {
	if(P == NULL)
		return NULL;
	return P->next;
}


