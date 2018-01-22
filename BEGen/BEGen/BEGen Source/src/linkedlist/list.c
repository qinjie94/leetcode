/*
 * list.c
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 *
 *      Author: Mark Allen Weiss (http://users.cis.fiu.edu/~weiss/dsaa_c2e/files.html)
 */

#include "list.h"


/* Place in the interface file */
struct Node {
	ElementType element;
	ListPosition next;
};

struct MetaList{
	ListPosition nodeList;
	ListPosition lastElement;
	int listSize;
};


List make_empty_LIST(List L, void (*free_element)(void*)) {
	if (L != NULL)
		delete_list_LIST(L, free_element);
	L = malloc(sizeof(struct MetaList));
	L->nodeList = malloc(sizeof(struct Node));
	L->listSize = 0;
	if (L == NULL)
		fatal_error( "Out of memory!" );

	L->nodeList->next = NULL;
	L->lastElement = NULL;
	return L;
	}


List make_empty_pointer_LIST(List L) {
	if (L != NULL)
		delete_list_pointer_LIST(L);
	L = malloc(sizeof(struct MetaList));
	L->nodeList = malloc(sizeof(struct Node));

	L->listSize = 0;
	if (L == NULL)
		fatal_error( "Out of memory!" );

	L->nodeList->next = NULL;
	L->lastElement = NULL;
	return L;
	}



int get_size_LIST(const List L) {
	if(L == NULL)
		return 0;
	return L->listSize;
}

int is_empty_LIST(const List L) {
	if(L == NULL)
		fatal_error("List is NULL!");
	return L->nodeList->next == NULL;
}


/* Parameter L is unused in this implementation */
int is_last_LIST(const ListPosition P) {
	if(P == NULL)
		return 1;

	return P->next == NULL;
}


/* Return ListPosition of X in L; NULL if not found */
ListPosition find_LIST(const ElementType X, const List L, int (*compare)(const void*, const void*)) {
	ListPosition P;

	if(L == NULL) return NULL;

	P = L->nodeList->next;

	while (P != NULL && compare(X, P->element) != 0)
		P = P->next;

	return P;
}


/* Delete from a list */
/* Cell pointed to by P->Next is wiped out */
/* Assume that the position is legal */
/* Assume use of a header node */
static void delete(ListPosition prevP, List L, void (*free_element)(void*)) {
	ListPosition TmpCell;

	if (!is_last_LIST(prevP)) /* Assumption of header use */
	{ /* X is found; delete it */
		TmpCell = prevP->next;
		prevP->next	= TmpCell->next;

		L->listSize--;
		free_element(TmpCell->element);
		free(TmpCell);
		TmpCell = NULL;

		if(is_last_LIST(prevP))
			L->lastElement = prevP;
	}

	if(L->listSize == 0){
		L->nodeList->next = NULL;
		L->lastElement = NULL;
	}
}


/* Delete from a list */
/* Cell pointed to by P->Next is wiped out */
/* Assume that the position is legal */
/* Assume use of a header node */
void delete_LIST(ElementType X, List L, int (*compare)(const void*, const void*), void (*free_element)(void*)) {
	ListPosition prevP;
	prevP = find_previous_LIST(X, L, compare);
	delete(prevP, L, free_element);
}



/* If X is not found, then Next field of returned value is NULL */
/* Assumes a header */
ListPosition find_previous_LIST(const ElementType X, const List L, int (*compare)(const void*, const void*)) {
	ListPosition P;

	P = L->nodeList;
	while (P->next != NULL && compare(X, P->next->element) != 0)
		P = P->next;

	return P;
}


/* Insert (after legal position P) */
/* Header implementation assumed */
/* Parameter L is unused in this implementation */
void insert_LIST(const ElementType X, List L, ListPosition P) {
	ListPosition TmpCell;

	TmpCell = malloc(sizeof(struct Node));
	if (TmpCell == NULL) {
		fatal_error( "Out of space!!!" );
	}

	TmpCell->element = X;
	TmpCell->next = P->next;
	P->next = TmpCell;
	L->listSize++;

	if(is_last_LIST(TmpCell))
		L->lastElement = TmpCell;
}


/* Insert (after legal position P) */
/* Header implementation assumed */
void insert_last_LIST(const ElementType X, List L) {
	ListPosition TmpCell;

	TmpCell = malloc(sizeof(struct Node));
	if (TmpCell == NULL) {
		fatal_error( "Out of space!!!" );
	}

	TmpCell->element = X;

	if(L->lastElement == NULL){
		L->nodeList->next = TmpCell;
	}else{
		L->lastElement->next = TmpCell;
	}

	TmpCell->next = NULL;

	L->listSize++;
	L->lastElement = TmpCell;
}


/* Insert (after legal position P) */
/* Header implementation assumed */
void insert_first_LIST(const ElementType X, List L) {
	ListPosition TmpCell;

	TmpCell = malloc(sizeof(struct Node));
	if (TmpCell == NULL) {
		fatal_error( "Out of space!!!" );
	}

	TmpCell->element = X;

	if(L->nodeList->next == NULL){
		L->nodeList->next = TmpCell;
		L->lastElement = TmpCell;
		TmpCell->next = NULL;
	}else{
		TmpCell->next = L->nodeList->next;
		L->nodeList->next = TmpCell;
	}

	L->listSize++;
}


List clone_LIST(List L, void* (*clone)(void*)){
	List newList;
	ListPosition pos;
	ElementType element;

	newList = make_empty_LIST(NULL, free);

	if(get_size_LIST(L) == 0)
		return NULL;

	pos = header_LIST(L);

	while((pos = advance_LIST(pos)) !=  NULL){
		element = clone(retrieve_LIST(pos));
		insert_last_LIST(element, newList);
	}


	return newList;
}


/* Insert (after legal position P) */
/* Header implementation assumed */
void insert_list_last_LIST(const List newL, List L) {
	ListPosition pos;
	ElementType element;

	if(get_size_LIST(newL) == 0)
		return;

	pos = header_LIST(newL);

	while((pos = advance_LIST(pos)) !=  NULL){
		element = retrieve_LIST(pos);
		insert_last_LIST(element, L);
	}
}



/* Insert (after legal position P) */
/* Header implementation assumed */
/* Parameter L is unused in this implementation */
void insert_list_first_LIST(const List newL, List L) {
	ListPosition pos;
	ElementType element;

	if(get_size_LIST(newL) == 0)
		return;

	pos = header_LIST(newL);

	while((pos = advance_LIST(pos)) !=  NULL){
		element = retrieve_LIST(pos);
		insert_first_LIST(element, L);
	}
}


int is_equal_LIST(const List L1, const List L2, int (*equal)(const void*, const void*)){
	ListPosition posL1;
	ListPosition posL2;

	if(get_size_LIST(L1) != get_size_LIST(L2))
		return 0;

	posL1 = header_LIST(L1);
	posL2 = header_LIST(L2);

	while((posL1 = advance_LIST(posL1)) !=  NULL){
		posL2 = advance_LIST(posL2);
		if(!equal(retrieve_LIST(posL1), retrieve_LIST(posL2)))
			return FALSE;
	}

	return TRUE;
}


/* Assume distinct element in each list */
int is_contained_LIST(const List L1, const List L2, int (*compare)(const void*, const void*)){
	ListPosition posL1;
	ListPosition posL2;

	int isFound;


	if(get_size_LIST(L1) != get_size_LIST(L2))
		return 0;

	posL1 = header_LIST(L1);

	while((posL1 = advance_LIST(posL1)) !=  NULL){
		posL2 = header_LIST(L2);

		isFound = 0;
		while((posL2 = advance_LIST(posL2)) !=  NULL){
			if(compare(retrieve_LIST(posL1), retrieve_LIST(posL2)) == 0){
				isFound = 1;
				break;
			}
		}

		if(!isFound)
			return 0;
	}

	return 1;
}


/* Assume distinct element in each list */
List find_discrepancies_LIST(const List L1, const List L2, int (*equal)(const void*, const void*)){
	ListPosition posL1;
	ListPosition posL2;
	List discrepancies;

	int isFound;
	discrepancies = make_empty_LIST( NULL, free );

	posL1 = header_LIST(L1);

	while((posL1 = advance_LIST(posL1)) !=  NULL){
		posL2 = header_LIST(L2);

		isFound = FALSE;
		while((posL2 = advance_LIST(posL2)) !=  NULL){
			if(equal(retrieve_LIST(posL1), retrieve_LIST(posL2))){
				if(!isFound){
					isFound = TRUE;
					break;
				}
			}
		}

		if(!isFound)
			insert_last_LIST(retrieve_LIST(posL1),discrepancies);
	}

	return discrepancies;
}



/* Correct DeleteList algorithm */
List delete_list_LIST(List L, void (*free_element)(void*)) {
	ListPosition P, Tmp;

	if(L == NULL)
		fatal_error("Double free() call!");

	/* Header assumed */
	P = L->nodeList->next;

	L->nodeList->next = NULL;
	while (P != NULL) {
		Tmp = P->next;
		free_element(P->element);
		free(P);
		P = Tmp;
	}

	free(L->nodeList);
	free(L);
	return NULL;
}


/* Correct DeleteList algorithm */
List delete_list_pointer_LIST(List L) {
	ListPosition P, Tmp;

	if(L == NULL)
		fatal_error("Double free() call!");

	/* Header assumed */
	P = L->nodeList->next;

	L->nodeList->next = NULL;
	while (P != NULL) {
		Tmp = P->next;
		free(P);
		P = Tmp;
	}

	free(L->nodeList);
	free(L);
	return NULL;
}



ListPosition header_LIST(const List L) {
	if(L != NULL)
		return L->nodeList;
	return NULL;
}

ListPosition first_LIST(const List L) {
	if(L != NULL)
		return L->nodeList->next;
	return NULL;
}

ListPosition advance_LIST(const ListPosition P) {
	if(P != NULL)
		return P->next;
	return NULL;
}


ListPosition advance_remove_LIST(const List L, const ListPosition prevP, const ListPosition currP, void (*free_element)(void*)) {

	ListPosition advancedP;

	if(currP != NULL)
		advancedP = currP->next;
	else
		advancedP = NULL;

	delete(prevP, L, free_element);

	return advancedP;
}

void remove_pos_LIST(const List L, const ListPosition prevP, void (*free_element)(void*)) {
	delete(prevP, L, free_element);
}

ElementType retrieve_LIST(const ListPosition P) {
	if(P != NULL)
		return P->element;
	return NULL;
}

ElementType retrieve_last_LIST(const List L) {
	if(L->lastElement != NULL)
		return L->lastElement->element;
	return NULL;
}

char* to_string_LIST(const List L, char* (*to_string)(const void* x)){
	char* buf;
    ListPosition P;

    buf = malloc(sizeof(char) * MAX_BUFFER_SIZE_FOR_PRINTING );
    strcpy(buf, "");

    if(L == NULL)
    	return buf;

    if(get_size_LIST(L) == 0)
    	return buf;

    P = header_LIST( L );
    strcpy(buf, "");
    while((P = advance_LIST(P)) != NULL){
    	strcat(buf, to_string(retrieve_LIST( P )));
    	strcat(buf, ",");
     }

	return buf;
}

