/*
 * hashseq.c
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 */

/* Interface for separate chaining hash table */
#include "../shared/generictype.h"
#include "../shared/fatal.h"
#include "stdint.h"

#ifndef _HashSep_H
#define _HashSep_H

typedef uint32_t Index;

struct ListNode;
typedef struct ListNode *HashPosition;
struct HashTbl;
typedef struct HashTbl *HashTable;
struct HashIterator;
typedef struct HashIterator *HashIterator;

HashTable create_HASH(int TableSize);
HashTable destroy_HASH(HashTable H, void (*free_key)(void*), void (*free_value)(void*));
HashPosition find_HASH(const ElementType Key, const HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*));
void insert_HASH(const ElementType Key, const ElementType Element, HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*));
void remove_HASH(const ElementType Key, HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*), void (*free_key)(void*), void (*free_value)(void*));
ElementType retrieve_key_HASH(const HashPosition P);
ElementType retrieve_value_HASH(const HashPosition P);
HashPosition advance_HASH(const HashPosition P);
HashIterator get_iterator_HASH(const HashTable H);
HashPosition iterator_next_HASH(const HashIterator I);
int iterator_has_next_HASH(HashIterator I);

HashTable clone_HASH(HashTable H, uint32_t  (*hash_key)(const void*), int (*compare)(const void*, const void*), void* (*clone_key)(void*), void* (*clone_value)(void*));

/* TODO: Routines such as Delete are MakeEmpty are omitted */

#endif
