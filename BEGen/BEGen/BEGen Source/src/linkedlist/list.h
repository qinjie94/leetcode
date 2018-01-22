/*
 * list.h
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
#include "../shared/fatal.h"
#include "../shared/constant.h"

#ifndef _List_H
#define _List_H

struct Node;
struct MetaList;
typedef struct Node *PtrToNode;
typedef struct MetaList *PtrToMetaList;
typedef PtrToMetaList List;
typedef PtrToNode ListPosition;

int get_size_LIST(const List L);
List make_empty_LIST( List L, void (*free_element)(void*));
List make_empty_pointer_LIST(List L);
int is_empty_LIST(const List L);
int is_last_LIST(const ListPosition P);
ListPosition find_LIST(const ElementType X, const List L, int (*compare)(const void*, const void*));
void delete_LIST(ElementType X, List L, int (*compare)(const void*, const void*), void (*free_element)(void*));
void remove_pos_LIST(const List L, const ListPosition prevP, void (*free_element)(void*));
ListPosition find_previous_LIST(const ElementType X, const List L, int (*compare)(const void*, const void*));
void insert_LIST(const ElementType X, List L, ListPosition P);
void insert_last_LIST(const ElementType X, List L);
void insert_first_LIST(const ElementType X, List L);
void insert_list_last_LIST(const List newL, List L);
void insert_list_first_LIST(const List newL, List L);
List delete_list_LIST(List L, void (*free_element)(void*));
List delete_list_pointer_LIST(List L);
ListPosition header_LIST(const List L);
ListPosition first_LIST(const List L);
ListPosition advance_LIST(const ListPosition P);
ListPosition advance_remove_LIST(const List L, const ListPosition prevP, const ListPosition currP, void (*free_element)(void*));
ElementType retrieve_LIST(const ListPosition P);
ElementType retrieve_last_LIST(const List L);
char* to_string_LIST( const List L, char* (*to_string)(const void* x) );
int compare_LIST(const List L1, const List L2, int (*compare)(const void*, const void*));
int is_contained_LIST(const List L1, const List L2, int (*compare)(const void*, const void*));
int is_equal_LIST(const List L1, const List L2, int (*equal)(const void*, const void*));
List find_discrepancies_LIST(const List L1, const List L2, int (*equal)(const void*, const void*));

List clone_LIST(List L, void* (*clone)(void*));
#endif

