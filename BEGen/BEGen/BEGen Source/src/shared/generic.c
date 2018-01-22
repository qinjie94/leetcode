/*
 * compare.c
 *
 *  Created on: Apr 29, 2009
 *      Author: Mo Sadoghi
 */
#include "generic.h"

#define INT_SIZE 5
#define FLOAT_SIZE 5


static DEFINE_HASHTABLE_INSERT(insert_index, uint32_t, RPredicateIndex);
static DEFINE_HASHTABLE_SEARCH(search_index, uint32_t, RPredicateIndex);

void delete_ptr_pointer_list(void* x){
	delete_list_pointer_LIST(x);
}

void delete_ptr_primitive_list(void* x){
	delete_list_LIST(x, free);
}


void delete_ptr_predicate(void* x){
	free(x);
}


void delete_ptr_zpredicate(void* x){
	free(x);
}

void delete_ptr_rpredicate(void* x){
	free(((RPredicate*)x)->values);
	free(x);
}


void delete_ptr_rpredicateStream(void* x){
	free(((RPredicateStream*)x)->values);
	free(x);
}


void dummy_free(void* x){
}


void* dummy_clone(void* x){
	return x;
}


void delete_ptr_subscription(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((Sub*)x)->predicate = delete_list_LIST(((Sub*)x)->predicate, delete_ptr_predicate);
	((Sub*)x)->subPredicate = destroy_HASH(((Sub*)x)->subPredicate, dummy_free, dummy_free);
	free(x);
}


void delete_ptr_zsubscription(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((ZSub*)x)->predicate = delete_list_LIST(((ZSub*)x)->predicate, delete_ptr_zpredicate);
	free(x);
}


void delete_ptr_object_nopred(void* sub){
	if(sub == NULL)
		fatal_error("DOUBLE FREE!");

	free(((Object*)sub)->predicate);
	free(((Object*)sub)->active);
	free(((Object*)sub)->covered);
	free(((Object*)sub)->score);
	free(sub);
}


void delete_ptr_object(void* sub){
	int i;
	if(sub == NULL)
		fatal_error("DOUBLE FREE!");

	for(i = 0; i < ((Object*)sub)->arraySize; i++)
		delete_ptr_rpredicate(((Object*)sub)->predicate[i]);

	free(((Object*)sub)->predicate);
	free(((Object*)sub)->active);
	free(((Object*)sub)->covered);
	free(((Object*)sub)->score);
	free(sub);
}


void delete_ptr_object_with_hash(void* sub){
	int i;
	if(sub == NULL)
		fatal_error("DOUBLE FREE!");

	for(i = 0; i < ((Object*)sub)->arraySize; i++)
		delete_ptr_rpredicate(((Object*)sub)->predicate[i]);

	free(((Object*)sub)->predicate);
	free(((Object*)sub)->active);
	free(((Object*)sub)->covered);
	free(((Object*)sub)->score);
	hashtable_destroy(((Object*)sub)->index, FALSE, free);
	free(sub);
}


void delete_ptr_object_nopred_with_hash(void* sub){
	if(sub == NULL)
		fatal_error("DOUBLE FREE!");

	free(((Object*)sub)->predicate);
	free(((Object*)sub)->active);
	free(((Object*)sub)->covered);
	free(((Object*)sub)->score);
	hashtable_destroy(((Object*)sub)->index, FALSE, free);
	free(sub);
}



void delete_ptr_objectStream(void* sub){
	int i;
	if(sub == NULL)
		fatal_error("DOUBLE FREE!");

	for(i = 0; i < ((ObjectStream*)sub)->arraySize; i++)
		delete_ptr_rpredicateStream(((ObjectStream*)sub)->predicate[i]);

	free(((ObjectStream*)sub)->predicate);
	free(((ObjectStream*)sub)->active);
	free(((ObjectStream*)sub)->covered);
	free(((ObjectStream*)sub)->score);
	free(sub);
}



void delete_ptr_zevent(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((ZEvent*)x)->eventPredicate = delete_list_LIST(((ZEvent*)x)->eventPredicate, delete_ptr_zpredicate);
	free(x);
}


void delete_ptr_event_nopred_nohash(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((Event*)x)->predicateList = delete_list_LIST(((Event*)x)->predicateList, dummy_free);
	free(x);
}


void delete_ptr_event_nopred(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((Event*)x)->predicateHash = destroy_HASH(((Event*)x)->predicateHash, dummy_free, dummy_free);
	((Event*)x)->predicateList = delete_list_LIST(((Event*)x)->predicateList, dummy_free);
	free(x);
}


void delete_ptr_event(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((Event*)x)->predicateHash = destroy_HASH(((Event*)x)->predicateHash, delete_ptr_rpredicate, dummy_free);
	free(x);
}



void delete_ptr_clone_subscription(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((Sub*)x)->predicate = delete_list_LIST(((Sub*)x)->predicate, dummy_free);
	((Sub*)x)->subPredicate = destroy_HASH(((Sub*)x)->subPredicate, dummy_free, dummy_free);
	free(x);
}


void delete_ptr_clone_zsubscription(void* x){
	if(x == NULL)
		fatal_error("DOUBLE FREE!");

	((ZSub*)x)->predicate = delete_list_LIST(((ZSub*)x)->predicate, dummy_free);
	free(x);
}

int equal_ptr_int(const void* x, const void* y){
	return compare_ptr_int(x, y) == 0;
}


int compare_uint16_t(const uint16_t x, const uint16_t y){
	if(x < y)
		return -1;
	else if(x > y)
		return 1;
	else
		return 0;
}

int compare_ptr_int(const void* x, const void* y){
	if(*(int*)x < *(int*)y)
		return -1;
	else if(*(int*)x > *(int*)y)
		return 1;
	else
		return 0;
}


int compare_ptr_uint8_t(const void* x, const void* y){
	if(*(uint8_t*)x < *(uint8_t*)y)
		return -1;
	else if(*(uint8_t*)x > *(uint8_t*)y)
		return 1;
	else
		return 0;
}

int compare_ptr_float(const void* x, const void* y){
	if(*(float*)x < *(float*)y)
		return -1;
	else if(*(float*)x > *(float*)y)
		return 1;
	else
		return 0;
}


int compare_reverse_ptr_float(const void* x, const void* y){
	if(*(float*)x > *(float*)y)
		return -1;
	else if(*(float*)x < *(float*)y)
		return 1;
	else
		return 0;
}


int compare_ptr_predicate(const void* x, const void* y){
	if((((Predicate*)x))->id < (((Predicate*)y))->id)
		return -1;
	if((((Predicate*)x))->id > (((Predicate*)y))->id)
		return 1;
	else
		return 0;
}

int compare_ptr_zpredicate(const void* x, const void* y){
	if((((ZPredicate*)x))->id < (((ZPredicate*)y))->id)
		return -1;
	if((((ZPredicate*)x))->id > (((ZPredicate*)y))->id)
		return 1;
	else{

		if((((ZPredicate*)x))->value < (((ZPredicate*)y))->value)
			return -1;
		if((((ZPredicate*)x))->value > (((ZPredicate*)y))->value)
			return 1;

		return 0;
	}
}



int compare_ptr_zpredicate_dim(const void* x, const void* y){
	if((((ZPredicate*)x))->id < (((ZPredicate*)y))->id)
		return -1;
	if((((ZPredicate*)x))->id > (((ZPredicate*)y))->id)
		return 1;
	return 0;
}


int compare_ptr_zpredicate_value(const void* x, const void* y){
	if((((ZPredicate*)x))->value < (((ZPredicate*)y))->value)
		return -1;
	if((((ZPredicate*)x))->value > (((ZPredicate*)y))->value)
		return 1;
	return 0;
}


int compare_ptr_subscription(const void* x, const void* y){
	if((((Sub*)x))->id < (((Sub*)y))->id)
		return -1;
	if((((Sub*)x))->id > (((Sub*)y))->id)
		return 1;
	else
		return 0;
}


int compare_ptr_object(const void* x, const void* y){
	if((((Object*)x))->id < (((Object*)y))->id)
		return -1;
	if((((Object*)x))->id > (((Object*)y))->id)
		return 1;
	else
		return 0;
}

int equal_ptr_object(const void* x, const void* y){
	return compare_ptr_object(x, y) == 0;
}


int equal_ptr_subscription(const void* x, const void* y){
	return compare_ptr_subscription(x, y) == 0;
}


int compare_ptr_zsubscription(const void* x, const void* y){
	if((((ZSub*)x))->id < (((ZSub*)y))->id)
		return -1;
	if((((ZSub*)x))->id > (((ZSub*)y))->id)
		return 1;
	else
		return 0;
}


char* to_string_ptr_int(const void* x){
	char* buf =  malloc(sizeof(char) * INT_SIZE);
	sprintf(buf, "%d", *(int*)x);
	return buf;
}


char* to_string_ptr_float(const void* x){
	char* buf =  malloc(sizeof(char) * FLOAT_SIZE);
	sprintf(buf, "%.2f", *(float*)x);
	return buf;
}


char* to_string_ptr_subscription(const void* x){
	char* buf =  malloc(sizeof(char) * FLOAT_SIZE);
	sprintf(buf, "%d", (((Sub*)x))->id);
	return buf;
}


char* to_string_ptr_zsubscription(const void* x){
	char* buf =  malloc(sizeof(char) * FLOAT_SIZE);
	sprintf(buf, "%d", (((ZSub*)x))->id);
	return buf;
}


char* to_string_ptr_predicate(const void* x){
	char* buf =  malloc(sizeof(char) * FLOAT_SIZE);
	sprintf(buf, "%d", (((Predicate*)x))->id);
	return buf;
}

char* to_string_ptr_zpredicate(const void* x){
	char* buf =  malloc(sizeof(char) * FLOAT_SIZE);
	sprintf(buf, "%d", (((ZPredicate*)x))->id);
	return buf;
}


uint32_t hash_key_ptr_int (const void* x){
	return *(int*)x;
}


uint32_t hash_key_ptr_predicate (const void* x){
	return (((Predicate*)x))->id;
}


uint32_t hash_key_ptr_zpredicate (const void* x){
	return (((Predicate*)x))->id;
}


float* get_min_float_value(){
	float *minFloat = malloc( sizeof( float ) );
	*minFloat = MIN_INT;
	return minFloat;
}

float* get_max_float_value(){
	float *maxFloat = malloc( sizeof( float ) );
	*maxFloat = MAX_INT;
	return maxFloat;
}


int* get_min_int_value(){
	int *minInt = malloc( sizeof( int ) );
	*minInt = MIN_INT;
	return minInt;
}



int* get_max_int_value(){
	int *maxInt = malloc( sizeof( int ) );
	*maxInt = MAX_INT;
	return maxInt;
}



void* clone_subscription(void* sub){
	Sub* newSub = malloc(sizeof(Sub));

	(((Sub*)newSub))->id = (((Sub*)sub))->id;
	(((Sub*)newSub))->predicate = clone_LIST((((Sub*)sub))->predicate, dummy_clone);
	(((Sub*)newSub))->subPredicate = clone_HASH((((Sub*)sub))->subPredicate, hash_key_ptr_predicate, compare_ptr_predicate, dummy_clone, dummy_clone);

	return newSub;

}


void* clone_zsubscription(void* sub){
	ZSub* newSub = malloc(sizeof(ZSub));

	newSub->id = (((ZSub*)sub))->id;
	newSub->predicate = clone_LIST((((ZSub*)sub))->predicate, dummy_clone);

	return newSub;

}


void* clone_object_to_zsub(void* sub){
	ZPredicate* zpred;
	int currPred;
	Object* oldSub = sub;
	ZSub* newSub = malloc(sizeof(ZSub));

	newSub->id = oldSub->id;
	newSub->predicate = make_empty_LIST( NULL, free );

	for(currPred = 0; currPred < oldSub->arraySize; currPred++){
		zpred = malloc(sizeof(ZPredicate));

		if(oldSub->predicate[currPred]->operator != OPT_EQ)
			fatal_error("ZSub only supports equality operator!");

		zpred->id = oldSub->predicate[currPred]->attribute;
		zpred->value = oldSub->predicate[currPred]->values[0];
		insert_last_LIST(zpred, newSub->predicate);
	}

	return newSub;
}



void* clone_event_to_zevent(void* event){
	RPredicate* rpred;
	ZPredicate* zpred;
	ListPosition subPredicate;
	Event* oldEvent = event;
	ZEvent* newEvent = malloc(sizeof(ZEvent));


	newEvent->id = oldEvent->id;
	newEvent->eventPredicate = make_empty_LIST( NULL, free );


	subPredicate = header_LIST( oldEvent->predicateList );

	/* Iterator through subscriptions list */
	while((subPredicate = advance_LIST( subPredicate )) != NULL){
		rpred = retrieve_LIST(subPredicate);
		zpred = malloc(sizeof(ZPredicate));

		zpred->id = rpred->attribute;
		zpred->value = rpred->values[0];
		insert_last_LIST(zpred, newEvent->eventPredicate);
	}

	return newEvent;
}



void* clone_object(void* sub){
	Object* oldSub = ((Object*)sub);
	Object* newSub = malloc(sizeof(Object));


	newSub->id = oldSub->id;
	newSub->arraySize = oldSub->arraySize;
	newSub->numEqualityLeft = oldSub->numEqualityLeft;

	newSub->predicate = malloc(sizeof(RPredicate*) * newSub->arraySize);
	newSub->active = malloc(sizeof(uint8_t) * newSub->arraySize);
	newSub->covered = malloc(sizeof(uint8_t) * newSub->arraySize);
	newSub->score = malloc(sizeof(PredScore) * newSub->arraySize);

	memcpy (newSub->predicate,oldSub->predicate,newSub->arraySize * sizeof(RPredicate*));
	memcpy (newSub->active,oldSub->active,newSub->arraySize * sizeof(uint8_t));
	memcpy (newSub->covered,oldSub->covered,newSub->arraySize * sizeof(uint8_t));
	memcpy (newSub->score,oldSub->score,newSub->arraySize * sizeof(PredScore));


	/* NOT A REAL CLONE; THUS IT SHOULD NOT BE REMOVED WHEN THE CLONE IS DESTROYED */
	/* TODO: If now if search the index for a removed predicate, then we will have a problem */
	newSub->index = oldSub->index;

	return newSub;
}


void index_object(Object *obj){
	uint32_t *currID;
	RPredicateIndex *currPredIndex;
	int i;

	obj->index = create_hashtable(obj->arraySize * 2, hash_identity, equal_uint32_keys);

	for(i=0; i < obj->arraySize; i++){
		currID = malloc(sizeof(uint32_t));
		*currID = obj->predicate[i]->attribute;

		currPredIndex = malloc(sizeof(RPredicateIndex));
		currPredIndex->predicate = obj->predicate[i];
		currPredIndex->scoreIndex = i;

		insert_index(obj->index, currID , currPredIndex);
	}
}



void match_event_sub(const Event* event, List subs, List matchedSub, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*)){
	ListPosition subList;
	ListPosition subPredicate;
	int isMatched;

	subList = header_LIST( subs );

	/* Iterator through subscriptions list */
	while((subList = advance_LIST( subList )) != NULL){

		subPredicate = header_LIST( ((Sub*)retrieve_LIST( subList ))->predicate );

		isMatched = TRUE;

		/* Iterator through each predicate of a subscription */
		while((subPredicate = advance_LIST( subPredicate )) != NULL){
			if(find_HASH(retrieve_LIST( subPredicate ), event->predicateHash, hash_key, compare) == NULL){
				isMatched = FALSE;
				break;
			}
		}

		if(isMatched){
			insert_last_LIST(retrieve_LIST( subList ), matchedSub);
		}

	}
}



void match_object_object_advance_topk(const Object* event, List subs, PriorityQueue matchedSubMinHeap, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*)){
	ListPosition subList;
	int isMatched;
	int currPredCounter, currPredValue;
	Object *currSub;
	RPredicate *eventPred, *subPred;
	uint32_t currPredID;
	RPredicateIndex *currEventPredIndex;
	PredScore eventPredScore;
	PredScore *currSubScore;

	subList = header_LIST( subs );


	/* Iterator through subscriptions list */
	while((subList = advance_LIST( subList )) != NULL){
		currSub = (Object*)retrieve_LIST( subList );
		isMatched = TRUE;
		currSubScore = malloc(sizeof(PredScore));
		*currSubScore = 0;

		for(currPredCounter = 0; currPredCounter < currSub->arraySize; currPredCounter++){
			subPred = currSub->predicate[currPredCounter];
			currPredID = subPred->attribute;
			currEventPredIndex = search_index(event->index, &currPredID);

			if(currEventPredIndex != NULL){
				eventPred = currEventPredIndex->predicate;
				eventPredScore = event->score[currEventPredIndex->scoreIndex];

				if(!currSub->covered[currPredCounter]){
					if(isBoundaryContained(&subPred->valuesBoundary, &eventPred->valuesBoundary)){
						/* Assume event is always equality */
						if(subPred->operator == OPT_NEQ && subPred->values[0] == eventPred->values[0]){
							isMatched = FALSE;
						}else if(subPred->operator == OPT_NOTIN){
							for(currPredValue = 0; currPredValue < subPred->valuesArraySize; currPredValue++){
								if(subPred->values[currPredValue] == eventPred->values[0]){
									isMatched = FALSE;
									break; /* break the inner loop */
								}else if(subPred->values[currPredValue] > eventPred->values[0]){
									break;
								}
							}
						}else if(subPred->operator == OPT_IN){
							isMatched = FALSE;
							for(currPredValue = 0; currPredValue < subPred->valuesArraySize; currPredValue++){
								if(subPred->values[currPredValue] == eventPred->values[0]){
									isMatched = TRUE;
									break; /* break the inner loop */
								}else if(subPred->values[currPredValue] > eventPred->values[0]){
									break;
								}
							}
						}
					}else{
						isMatched = FALSE;
					}
				}
				if(!isMatched) break; /* stop checking the rest of subscription predicate --> UNSAT*/
				else *currSubScore += currSub->score[currPredCounter] * eventPredScore;
			}else{
				isMatched = FALSE;
				break;
			}
		}

		if(isMatched){
			insert_max_k_BINHEAP(currSubScore, currSub, matchedSubMinHeap, compare_ptr_float, free);
		}else{
			free(currSubScore);
		}

	}
}



void match_object_object_advance(const Object* event, List subs, List matchedSub, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*)){
	ListPosition subList;
	int isMatched;
	int currPredCounter, currPredValue;
	Object *currSub;
	RPredicate *eventPred, *subPred;
	uint32_t currPredID;
	RPredicateIndex *currEventPredIndex;

	subList = header_LIST( subs );


	/* Iterator through subscriptions list */
	while((subList = advance_LIST( subList )) != NULL){
		currSub = (Object*)retrieve_LIST( subList );
		isMatched = TRUE;

		for(currPredCounter = 0; currPredCounter < currSub->arraySize; currPredCounter++){
			if(!currSub->covered[currPredCounter]){
				subPred = currSub->predicate[currPredCounter];
				currPredID = subPred->attribute;
				currEventPredIndex = search_index(event->index, &currPredID);

				if(currEventPredIndex != NULL){
					eventPred = currEventPredIndex->predicate;
					if(isBoundaryContained(&subPred->valuesBoundary, &eventPred->valuesBoundary)){
						/* Assume event is always equality */
						if(subPred->operator == OPT_NEQ && subPred->values[0] == eventPred->values[0]){
							isMatched = FALSE;
						}else if(subPred->operator == OPT_NOTIN){
							for(currPredValue = 0; currPredValue < subPred->valuesArraySize; currPredValue++){
								if(subPred->values[currPredValue] == eventPred->values[0]){
									isMatched = FALSE;
									break; /* break the inner loop */
								}else if(subPred->values[currPredValue] > eventPred->values[0]){
									break;
								}
							}
						}else if(subPred->operator == OPT_IN){
							isMatched = FALSE;
							for(currPredValue = 0; currPredValue < subPred->valuesArraySize; currPredValue++){
								if(subPred->values[currPredValue] == eventPred->values[0]){
									isMatched = TRUE;
									break; /* break the inner loop */
								}else if(subPred->values[currPredValue] > eventPred->values[0]){
									break;
								}
							}
						}
					}else{
						isMatched = FALSE;
					}
				}else{
					isMatched = FALSE;
				}
				if(!isMatched) break; /* stop checking the rest of subscription predicate --> UNSAT*/
			}
		}

		if(isMatched){
			insert_last_LIST(retrieve_LIST( subList ), matchedSub);
		}

	}
}



void match_event_object(const Event* event, List subs, List matchedSub, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*)){
	ListPosition subList;
	int isMatched;
	int currPredCounter, currPredValue;
	Object *currSub;
	RPredicate *eventPred, *subPred;

	subList = header_LIST( subs );


	/* Iterator through subscriptions list */
	while((subList = advance_LIST( subList )) != NULL){
		currSub = (Object*)retrieve_LIST( subList );
		isMatched = TRUE;

		for(currPredCounter = 0; currPredCounter < currSub->arraySize; currPredCounter++){
			if(!currSub->covered[currPredCounter]){
				subPred = currSub->predicate[currPredCounter];

				if(event->predicateHash != NULL){
					eventPred = retrieve_key_HASH(find_HASH(subPred, event->predicateHash, hash_key, compare));
				}else{
					eventPred = retrieve_LIST(find_LIST(subPred, event->predicateList, compare));
				}

				if(eventPred != NULL){
					if(isBoundaryContained(&subPred->valuesBoundary, &eventPred->valuesBoundary)){
						/* Assume event is always equality */
						if(subPred->operator == OPT_NEQ && subPred->values[0] == eventPred->values[0]){
							isMatched = FALSE;
						}else if(subPred->operator == OPT_NOTIN){
							for(currPredValue = 0; currPredValue < subPred->valuesArraySize; currPredValue++){
								if(subPred->values[currPredValue] == eventPred->values[0]){
									isMatched = FALSE;
									break; /* break the inner loop */
								}else if(subPred->values[currPredValue] > eventPred->values[0]){
									break;
								}
							}
						}else if(subPred->operator == OPT_IN){
							isMatched = FALSE;
							for(currPredValue = 0; currPredValue < subPred->valuesArraySize; currPredValue++){
								if(subPred->values[currPredValue] == eventPred->values[0]){
									isMatched = TRUE;
									break; /* break the inner loop */
								}else if(subPred->values[currPredValue] > eventPred->values[0]){
									break;
								}
							}
						}
					}else{
						isMatched = FALSE;
					}
				}else{
					isMatched = FALSE;
				}

//				if(find_LIST(currSub->predicate[currPredCounter], event->predicateList, compare) == NULL){
//					isMatched = FALSE;
//					break;
//				}
				if(!isMatched) break; /* stop checking the rest of subscription predicate --> UNSAT*/
			}
		}

		if(isMatched){
			insert_last_LIST(retrieve_LIST( subList ), matchedSub);
		}

	}
}



void match_zevent_zsub(const ZEvent* event, List subs, List matchedSub, int (*compare)(const void*, const void*)){
	ListPosition subList;
	ListPosition subPredicate;
	int isMatched;

	subList = header_LIST( subs );

	/* Iterator through subscriptions list */
	while((subList = advance_LIST( subList )) != NULL){

		subPredicate = header_LIST( ((ZSub*)retrieve_LIST( subList ))->predicate );

		isMatched = TRUE;

		/* Iterator through each predicate of a subscription */
		while((subPredicate = advance_LIST( subPredicate )) != NULL){
			if(find_LIST(retrieve_LIST( subPredicate ), event->eventPredicate, compare) == NULL){
				isMatched = FALSE;
				break;
			}
		}

		if(isMatched){
			insert_last_LIST(retrieve_LIST( subList ), matchedSub);
		}

	}
}


void ini_workload_parameter(WorkloadParameter *workloadParameter){
	workloadParameter->objectCount  = 0;
	workloadParameter->objectMaxDim  = 0;
	workloadParameter->objectOleq  = 0;
	workloadParameter->objectOgeq  = 0;
	workloadParameter->objectOeq  = 0;
	workloadParameter->objectOneq  = 0;
	workloadParameter->objectOlgeq  = 0;
	workloadParameter->objectOin  = 0;
	workloadParameter->objectOnotin  = 0;
	workloadParameter->isInforceOeq  = 0;
	workloadParameter->objectMaxExtendedObjectSize  = 0;
	workloadParameter->objectMaxNumberInNotIn  = 0;
	workloadParameter->objectUniform  = 0;
	workloadParameter->objectZip  = 0;
	workloadParameter->objectZipAlpha  = 0;
	workloadParameter->objectUniformModel  = 0;
	workloadParameter->objectWorkloadMatchingDegree  = 0;
	workloadParameter->isInforceObjectDim  = 0;
	strcpy(workloadParameter->objectOutput, "");
	workloadParameter->systemDim  = 0;
	workloadParameter->systemDimMeanScoreRatio  = 0;
	workloadParameter->systemDimVarianceScoreRatio  = 0;
	workloadParameter->systemDistinctValue  = 0;
	workloadParameter->systemMinDistinctValue  = 0;
	workloadParameter->systemDistinctValueUniform  = 0;
	workloadParameter->systemDistinctValueZip  = 0;
	workloadParameter->systemDistinctValueZipAlpha  = 0;
	workloadParameter->distinctValueIndex  = 0;
	workloadParameter->systemObjectOptCount  = 0;
	workloadParameter->isEqualityRanking  = 0;
}


void ini_experiment_parameter(ExperimentParameter* experimentParameter){
	experimentParameter->profileID= 0;
	experimentParameter->isRand = 0;
	strcpy(experimentParameter->argument, "");
	experimentParameter->subCount = 0;
	experimentParameter->subMaxDim = 0;
	experimentParameter->subOleq = 0;
	experimentParameter->subOgeq = 0;
	experimentParameter->subOeq = 0;
	experimentParameter->subOneq = 0;
	experimentParameter->subOlgeq = 0;
	experimentParameter->subOin = 0;
	experimentParameter->subOnotin = 0;
	experimentParameter->subMaxExtendedObjectSize = 0;
	experimentParameter->subMaxNumberInNotIn = 0;
	experimentParameter->subUniform = 0;
	experimentParameter->subZip = 0;
	experimentParameter->subUniformModel = 0;
	experimentParameter->subWorkloadMatchingDegree = 0;
	experimentParameter->isInforceSubDim = 0;
	experimentParameter->subOeqRatio = 0;
	strcpy(experimentParameter->subOutput, "");
	strcpy(experimentParameter->subInput, "");
	experimentParameter->matchingType = MATCHING_TYPE_STABBING_SUBSCRIPTION_EQUALITY_ONLY_EVENT;

	experimentParameter->eventCount = 0;
	experimentParameter->eventMaxDim = 0;
	experimentParameter->eventOeq = 0;
	experimentParameter->eventUniform = 0;
	experimentParameter->eventZip = 0;
	experimentParameter->eventUniformModel = 0;
	experimentParameter->eventChosenFromSub = 0;
	experimentParameter->isInforceEventDim = 0;
	experimentParameter->isEventHashEnabled = 0;
	strcpy(experimentParameter->eventOutput, "");
	strcpy(experimentParameter->eventInput, "");

	experimentParameter->systemDim = 0;
	experimentParameter->systemDimMeanScoreRatio = 0;
	experimentParameter->systemDimVarianceScoreRatio = 0;
	experimentParameter->systemDistinctValue = 0;
	experimentParameter->systemDistinctValueUniform = 0;
	experimentParameter->systemDistinctValueZip = 0;
	experimentParameter->systemDistinctValueZipAlpha = 0;
	experimentParameter->systemMinDistinctValue = 0;

	experimentParameter->scoringType = SCORING_ATTRIBUTE_FREQUENCY;
	experimentParameter->dataDistributionType = DERIVED_OBJECTS_SELECTED_RANDOMLY;

	experimentParameter->hashFunc = HASH_FUNC_NAIVE;
}


void ini_workload_stat(WorkloadStat* workloadStat){
	workloadStat->eqObjectCount = 0;
	workloadStat->avgEqOpCount = 0;
	workloadStat->avgNonEqOpCount= 0;
	workloadStat-> avgObjectDim = 0;
}




inline void ini_longdouble_2d_array(long double **array, int row, int col){
	int i,j;

	for(i = 1; i < row; i++){
		for(j = 1; j < col; j++){
			array[i][j] = 0;
		}
	}
}


inline void ini_longdouble_1d_array(long double *array, int size){
	int i;

	for(i = 1; i < size; i++){
		array[i] = 0;
	}
}


inline void ini_uint32_2d_array(uint32_t **array, int row, int col){
	int i,j;

	for(i = 1; i < row; i++){
		for(j = 1; j < col; j++){
			array[i][j] = 0;
		}
	}
}


inline long double** create_longdouble_2d_array(int row, int col){
	int i;
	long double **array;

	/* Create stats array */
	array = malloc(sizeof(long double*) * row);


	/* create internal partitions/clusters for stats array and ClusterPropagation */
	for(i = 1; i <  row; i++){
		array[i] = malloc(sizeof(long double) * col);
	}

	return array;
}



inline uint32_t** create_uint32_2d_array(uint32_t row, uint32_t col){
	int i;
	uint32_t **array;

	/* Create stats array */
	array = malloc(sizeof(uint32_t*) * row);


	/* create internal partitions/clusters for stats array and ClusterPropagation */
	for(i = 1; i <  row; i++){
		array[i] = malloc(sizeof(uint32_t) * col);
	}

	return array;
}


inline void free_uint32_2d_array(uint32_t **array, int row, int col){
	int i;

	if(array != NULL){
		/* free internal partitions/clusters for stats array and ClusterPropagation */
		for(i = 1; i <  row; i++){
			if(array[i] != NULL);
				free(array[i]);
		}

		/* Free stats array */
		free(array);
	}
}


inline void free_longdoubl_2d_array(long double **array, int row, int col){
	int i;

	if(array != NULL){
		/* free internal partitions/clusters for stats array and ClusterPropagation */
		for(i = 1; i <  row; i++){
			if(array[i] != NULL);
				free(array[i]);
		}

		/* Free stats array */
		free(array);
	}
}


void print_2d_array(int **array, int row, int col){
	int i,j;

	for(i = 1; i < row; i++){
		printf("row: %d, ", i);
		for(j = 1; j < col; j++){
			printf("%d ", array[i][j]);
		}
		puts("\n");
	}
}

void ini_2d_predvalue_array_null(PredValue** array, int arraySize){
	int i;

	for(i = 0; i < arraySize; i++){
		array[i] = NULL;
	}
}

void ini_predvalue_array(PredValue* array, int arraySize){
	int i;
	for(i = 0; i < arraySize; i++){
		array[i] = 0;
	}
}


void ini_predscore_array(PredScore* array, int arraySize){
	memset(array, 0, arraySize * sizeof(PredScore));
}



void ini_predvalue_array_to_value(PredValue* array, int arraySize, int defaultValue){
	int i;
	for(i = 0; i < arraySize; i++){
		array[i] = defaultValue;
	}
}


inline int get_dim_predicate(const void* x){
	return ((Predicate*)x)->id / MAX_NUMBER_OF_DIMENSION;
}


inline int get_value_predicate(const void* x){
	return ((Predicate*)x)->id % MAX_NUMBER_OF_DIMENSION;
}


inline int get_dim_zpredicate(const void* x){
	return ((ZPredicate*)x)->id;
}


inline int get_value_zpredicate(const void* x){
	return ((ZPredicate*)x)->value;
}


/*
inline uint16_t get_dim_rpredicate(const void* x){
	return ((RPredicate*)x)->attribute;
}


inline uint32_t* get_value_rpredicate(const void* x){
	return ((RPredicate*)x)->values;
}
*/


void remove_predicate_subscription(Sub* sub, Predicate* pred, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*)){
	//printf("Removing predicate: %d from sub %d \n", pred->id, sub->id);
	delete_LIST(pred, sub->predicate, compare, dummy_free);
	remove_HASH(pred, sub->subPredicate, hash_key, compare, dummy_free, dummy_free);
}


void remove_predicate_zsubscription(ZSub* sub, ZPredicate* pred, int (*compare)(const void*, const void*)){
	//printf("Removing predicate: %d from sub %d \n", pred->id, sub->id);
	delete_LIST(pred, sub->predicate, compare, dummy_free);
}


int equal_obj_keys(const void *k1, const void *k2){
	const Object *currObject1 = k1;
	const Object *currObject2 = k2;

    return (0 == memcmp(&currObject1->id, &currObject2->id, sizeof(uint32_t)));
}


int equal_uint32_keys(const void *k1, const void *k2){
    return (0 == memcmp(k1,k2,sizeof(uint32_t)));
}


int equal_uint16_keys(const void *k1, const void *k2){
    return (0 == memcmp(k1,k2,sizeof(uint16_t)));
}

int compare_str_keys(const void *k1, const void *k2){
	const char *str1 = k1;
	const char *str2 = k2;

	return strcmp(str1, str2);
}


int is_equal_str_keys(const void *k1, const void *k2){
	return compare_str_keys(k1, k2) == 0;
}


int compare_rpredicate_keys(const void *k1, const void *k2){
	int i = 0;
    if(((RPredicate*)k1)->id == ((RPredicate*)k2)->id){
    	if(((RPredicate*)k1)->attribute == ((RPredicate*)k2)->attribute){
			if(((RPredicate*)k1)->operator == ((RPredicate*)k2)->operator){
				if(((RPredicate*)k1)->valuesArraySize == ((RPredicate*)k2)->valuesArraySize){
					for(i = 0; i < ((RPredicate*)k1)->valuesArraySize; i++){
						if(((RPredicate*)k1)->values[i] != ((RPredicate*)k2)->values[i]){
							return -1;
						}
					}
					return 0;
				}
			}
    	}
    }
    return -1;
}


int compare_attr_value(const void *k1, const void *k2){
	if(((AttrValuePair*)k1)->attribute == ((AttrValuePair*)k2)->attribute){
		if(((AttrValuePair*)k1)->value == ((AttrValuePair*)k2)->value){
			return 1;
		}
	}
    return 0;
}



int is_equal_zevent_event(const void *zevent, const void *event){

	ListPosition predZEvent, predEvent;
	const ZEvent *currZEvent;
	const Event *currEvent;
	RPredicate *currRPred;
	ZPredicate *currZPred;
	int currRPredCounter = 0;

	currZEvent = zevent;
	currEvent = event;

	if(get_size_LIST(currEvent->predicateList) != get_size_LIST(currZEvent->eventPredicate))
		return FALSE;

	predZEvent = header_LIST( currZEvent->eventPredicate );
	predEvent = header_LIST( currEvent->predicateList );


	/* Iterator through subscriptions list */
	while((predZEvent = advance_LIST( predZEvent )) != NULL){
		predEvent = advance_LIST( predEvent );

		currZPred = retrieve_LIST(predZEvent);
		currRPred = retrieve_LIST(predEvent);

		if(currRPred->attribute != currZPred->id || currRPred->values[0] != currZPred->value || currRPred->operator != OPT_EQ)
			return FALSE;
		currRPredCounter++;
	}

	return TRUE;
}


int is_equal_object_object(const void *object1, const void *object2){

	const Object *currObject1;
	const Object *currObject2;
	RPredicate *currPredO1;
	RPredicate *currPredO2;
	int predCounter, valueCounter;

	currObject1 = object1;
	currObject2 = object2;

	if(currObject1->arraySize != currObject2->arraySize){
		error("ArraySize mismatch");
		return FALSE;
	}

	if(currObject1->id != currObject2->id){
		error("Object id mismatch!");
		return FALSE;
	}

	if(currObject1->numEqualityLeft != currObject2->numEqualityLeft){
		error("Object numEqualityLeft mismatch!");
		return FALSE;
	}


	for(predCounter = 0; predCounter < currObject1->arraySize; predCounter++){
		currPredO1 = currObject1->predicate[predCounter];
		currPredO2 = currObject2->predicate[predCounter];

		if(currObject1->active[predCounter] != currObject2->active[predCounter]){
			error("Object active mismatch!");
			return FALSE;
		}

		if(currObject1->covered[predCounter] != currObject2->covered[predCounter]){
			error("Object covered mismatch!");
			return FALSE;
		}

		if(currPredO1->id != currPredO2->id){
			error("Pred id mismatch!");
			return FALSE;
		}

		if(currPredO1->operator != currPredO2->operator){
			error("Pred operator mismatch!");
			return FALSE;
		}

		if(currPredO1->attribute != currPredO2->attribute){
			error("Pred attribute mismatch!");
			return FALSE;
		}

		if(currPredO1->valuesArraySize != currPredO2->valuesArraySize){
			error("Pred valuesArraySize mismatch!");
			return FALSE;
		}

		if(!isBoundaryEqual(&currPredO1->valuesBoundary, &currPredO2->valuesBoundary)){
			error("Pred boundary mismatch!");
			return FALSE;
		}

		for(valueCounter = 0; valueCounter < currPredO1->valuesArraySize; valueCounter++){
			if(currPredO1->values[valueCounter] != currPredO2->values[valueCounter]){
				error("Pred values mismatch!");
				return FALSE;
			}
		}
	}

	return TRUE;
}

int is_equal_1d_array(const int *array1, const int *array2, int arraySize){
	int i;

	for(i = 0; i < arraySize; i++)
		if(array1[i] != array2[i])
			return FALSE;

	return TRUE;
}

int is_equal_zsub_rsub(const void *zsub, const void *asub){

	ListPosition subList;
	const ZSub *currZSub;
	const Object *currASub;
	RPredicate *currRPred;
	ZPredicate *currZPred;
	int currRPredCounter = 0;

	currZSub = zsub;
	currASub = asub;

	if(currASub->arraySize != get_size_LIST(currZSub->predicate))
		return FALSE;

	subList = header_LIST( currZSub->predicate );


	/* Iterator through subscriptions list */
	while((subList = advance_LIST( subList )) != NULL){
		currZPred = retrieve_LIST(subList);
		currRPred = currASub->predicate[currRPredCounter];
		if(currRPred->attribute != currZPred->id || currRPred->values[0] != currZPred->value || currRPred->operator != OPT_EQ)
			return FALSE;
		currRPredCounter++;
	}

	return TRUE;
}



int compare_rpredicate_attr_only(const void *k1, const void *k2){
   	if(((RPredicate*)k1)->attribute == ((RPredicate*)k2)->attribute){
		return 0;
    }
    return -1;
}


int equal_rpredicate_keys(const void *k1, const void *k2){
	if(!compare_rpredicate_keys(k1, k2))
		return 1;
	return 0;
}

int equal_memory_location(const void *k1, const void *k2){
	if(k1 == k2)
		return 1;
	return 0;
}


uint32_t hash_thomas_wang_str(const void* key){
	uint32_t hashVal;
	const char *str;

	hashVal = 0;
	str = key;

	while (*str != '\0')
		hashVal += *str++;

	return hash_thomas_wang(hashVal);
}



uint32_t hash_thomas_wang_rpredicate(const void* pred){
	return hash_thomas_wang(((RPredicate*)pred)->id);
}


uint32_t hash_robert_jenkins_rpredicate(const void* pred){
        return hash_robert_jenkins(((RPredicate*)pred)->id);
}


uint32_t hash_thomas_wang_rpredicate_attr_only(const void* pred){
	return hash_thomas_wang(((RPredicate*)pred)->attribute);
}

uint32_t hash_thomas_wang_attr_value(const void* pred){
	return hash_thomas_wang(((AttrValuePair*)pred)->attribute + ((AttrValuePair*)pred)->value);
}


uint32_t hash_identity_rpredicate_attr_only(const void* pred){
	return ((RPredicate*)pred)->attribute;
}


uint32_t hash_identity_zpredicate_attr_only(const void* pred){
	return ((ZPredicate*)pred)->id;
}


uint32_t hash_identity_attr_only(const void* pred){
	return ((RPredicate*)pred)->attribute;
}

uint32_t hash_identity(const void* num){
	return (*(uint32_t*)num);
}


uint32_t hash_thomas_wang_num(const void* num){
	return hash_thomas_wang(*(uint32_t*)num);
}


uint32_t hash_thomas_wang(const uint32_t raw){
	uint32_t num = raw;

    num = (num ^ 61) ^ (num >> 16);
    num = num + (num << 3);
    num = num ^ (num >> 4);
    num = num * 0x27d4eb2d;
    num = num ^ (num >> 15);
    return num;
}


uint32_t hash_robert_jenkins(const uint32_t v){
	uint32_t num = v;

	num = (num+0x7ed55d16) + (num<<12);
	num = (num^0xc761c23c) ^ (num>>19);
	num = (num+0x165667b1) + (num<<5);
	num = (num+0xd3a2646c) ^ (num<<9);
	num = (num+0xfd7046c5) + (num<<3);
	num = (num^0xb55a4f09) ^ (num>>16);
	return num;
}


uint8_t get_min_uint8_t(uint8_t n1, uint8_t n2){
	if(n1 < n2)
		return n1;
	return n2;
}

uint8_t get_max_uint8_t(uint8_t n1, uint8_t n2){
	if(n1 < n2)
		return n2;
	return n1;
}


uint32_t get_max_uint32_t(uint32_t n1, uint32_t n2){
	if(n1 < n2)
		return n2;
	return n1;
}


PredScore get_max_predscore(PredScore n1, PredScore n2){
	if(n1 < n2)
		return n2;
	return n1;
}


PredScore get_min_predscore(PredScore n1, PredScore n2){
	if(n1 < n2)
		return n1;
	return n2;
}



uint32_t get_positive_predvalue_t(int n1){
	if(n1 < 0)
		return 0;
	return n1;
}


int get_max_int(int n1, int n2){
	if(n1 < n2)
		return n2;
	return n1;
}


int get_min_int(int n1, int n2){
	if(n1 < n2)
		return n1;
	return n2;
}


uint32_t get_min_uint32_t(uint32_t n1, uint32_t n2){
	if(n1 < n2)
		return n1;
	return n2;
}


PredValue get_min_predvalue_t(PredValue n1, PredValue n2){
	if(n1 < n2)
		return n1;
	return n2;
}

PredValue get_max_predvalue_t(PredValue n1, PredValue n2){
	if(n1 < n2)
		return n2;
	return n1;
}



inline struct boundary* createNewBoundary(PredValue minBoundary, PredValue maxBoundary){
	struct boundary *bound = malloc(sizeof(struct boundary));
	setBoundary(bound, minBoundary, maxBoundary);
	return bound;
}


inline void setBoundary(struct boundary *bound, PredValue minBoundary, PredValue maxBoundary){
	bound->minBoundary = minBoundary;
	bound->maxBoundary = maxBoundary;
}


inline void copyBoundary(struct boundary *destination, struct boundary *source){
	destination->minBoundary = source->minBoundary;
	destination->maxBoundary = source->maxBoundary;
}


inline int isBoundaryConflicted(struct boundary *leftBound, struct boundary *rightBound){
	/* NOT CONTAINED */
	if(leftBound->maxBoundary < rightBound->minBoundary)
		return FALSE;
	return TRUE;
}


inline int isBoundaryOverlapped(struct boundary *firstBound, struct boundary *secondBound){
	/* NOT CONTAINED WITH NON EMPTY OVERLAP */
	if(firstBound->minBoundary > secondBound->maxBoundary || firstBound->maxBoundary < secondBound->minBoundary)
		return FALSE;
	return TRUE;
}

inline int isBoundaryConstraintPassed(struct boundary *newBound, struct boundary *leftBound, struct boundary *rightBound){
	if(!isBoundaryValid(leftBound) || !isBoundaryValid(rightBound)){
		return TRUE;
	}else if(newBound->minBoundary < leftBound->maxBoundary && newBound->maxBoundary > rightBound->minBoundary){
		return FALSE;
	}
	return TRUE;
}

inline PredValue getExpansionRate(struct boundary *mainBoundary, struct boundary *object){
	int expansion = 0;

	if(!isBoundaryValid(mainBoundary)){
		expansion = object->maxBoundary - object->minBoundary;
	}

	if(object->minBoundary < mainBoundary->minBoundary){
		expansion += mainBoundary->minBoundary - object->minBoundary;
	}

	if(object->maxBoundary > mainBoundary->maxBoundary){
		expansion += object->maxBoundary - mainBoundary->maxBoundary;
	}

	return expansion;
}


inline PredValue getExpandedArea(struct boundary *mainBoundary, struct boundary *object){
	struct boundary tmp;

	if(!isBoundaryValid(mainBoundary)){
		setBoundary(&tmp, object->minBoundary, object->maxBoundary);
	}else{
		setBoundary(&tmp, mainBoundary->minBoundary, mainBoundary->maxBoundary);
		updateBoundary(&tmp, object);
	}

	assert(getBoundaryArea(&tmp) - getBoundaryArea(mainBoundary) >= 0);

	return getBoundaryArea(&tmp) - getBoundaryArea(mainBoundary);
}


inline int isBoundaryNonEmptyOverlapped(struct boundary *firstBound, struct boundary *secondBound){
	if(!isBoundaryValid(firstBound) || !isBoundaryValid(secondBound))
		return FALSE;

	/* NOT CONTAINED */
	if(firstBound->minBoundary > secondBound->maxBoundary || firstBound->maxBoundary < secondBound->minBoundary)
		return FALSE;
	else if(firstBound->maxBoundary == secondBound->minBoundary || firstBound->minBoundary == secondBound->maxBoundary)
		return FALSE;
	return TRUE;
}


inline int isMustRightBoundary(struct boundary *rightBound, struct boundary *object){
	if(!isBoundaryValid(rightBound))
		return FALSE;

	/* NOT CONTAINED */
	if(object->minBoundary >= rightBound->maxBoundary)
		return TRUE;

	return FALSE;
}

inline int isMustLeftBoundary(struct boundary *leftBound, struct boundary *object){
	if(!isBoundaryValid(leftBound))
		return FALSE;

	/* NOT CONTAINED */
	if(object->maxBoundary <= leftBound->minBoundary)
		return TRUE;

	return FALSE;
}


inline int isBoundaryContained(struct boundary *mainBound, struct boundary *childBound){
	if(isBoundaryValid(mainBound) && !isBoundaryValid(childBound))
		return TRUE;
	else if(!isBoundaryValid(mainBound) && isBoundaryValid(childBound))
		return FALSE;
	else if(!isBoundaryValid(mainBound) && !isBoundaryValid(childBound))
		return TRUE;

	if(mainBound->minBoundary <= childBound->minBoundary && mainBound->maxBoundary >= childBound->maxBoundary)
		return TRUE;
	return FALSE;
}

inline int isBoundaryCrossHyperplane(struct boundary *objBoundary, PredValue hyperplane){
	if(objBoundary->minBoundary < hyperplane && objBoundary->maxBoundary > hyperplane)
		return TRUE;
	return FALSE;
}



inline int isBoundaryLeftOfHyperplane(struct boundary *objBoundary, PredValue hyperplane){
	if(objBoundary->maxBoundary <= hyperplane)
		return TRUE;
	return FALSE;
}


inline int isBoundaryRightOfHyperplane(struct boundary *objBoundary, PredValue hyperplane){
	if(objBoundary->minBoundary >= hyperplane)
		return TRUE;
	return FALSE;
}


inline int isBoundaryEqual(struct boundary *mainBound, struct boundary *childBound){
	if(isBoundaryValid(mainBound) && !isBoundaryValid(childBound))
		return TRUE;
	else if(!isBoundaryValid(mainBound) && isBoundaryValid(childBound))
		return FALSE;
	else if(!isBoundaryValid(mainBound) && !isBoundaryValid(childBound))
		return TRUE;

	if(mainBound->minBoundary == childBound->minBoundary && mainBound->maxBoundary == childBound->maxBoundary)
		return TRUE;
	return FALSE;
}


inline void updateBoundary(struct boundary *mainBound, struct boundary *addedBound){

	assert(isBoundaryValid(addedBound));

	if(isBoundaryValid(mainBound)){
		mainBound->minBoundary = get_min_predvalue_t(mainBound->minBoundary, addedBound->minBoundary);
		mainBound->maxBoundary = get_max_predvalue_t(mainBound->maxBoundary, addedBound->maxBoundary);
	}else{
		mainBound->minBoundary = addedBound->minBoundary;
		mainBound->maxBoundary = addedBound->maxBoundary;
	}
}


inline PredValue getBoundaryArea(struct boundary *mainBound){
	if(isBoundaryValid(mainBound))
		return mainBound->maxBoundary - mainBound->minBoundary;
	return 0;
}


inline PredValue getRightOverlapBoundary(struct boundary *overlap, struct boundary *right){
	if(isBoundaryValid(overlap) && isBoundaryValid(right) && isBoundaryOverlapped(overlap, right)){
		return overlap->maxBoundary - right->minBoundary;
	}
	return 0;
}


inline PredValue getLeftOverlapBoundary(struct boundary *overlap, struct boundary *left){
	if(isBoundaryValid(overlap) && isBoundaryValid(left) && isBoundaryOverlapped(overlap, left)){
		return left->maxBoundary - overlap->minBoundary;
	}
	return 0;
}


inline int isBoundaryValid(struct boundary *mainBound){
	return mainBound->maxBoundary >= mainBound->minBoundary;
}


inline int isContinuousOp(RPredicate *pred){
	if(pred->operator == OPT_NEQ || pred->operator == OPT_IN || pred->operator == OPT_NOTIN)
		return FALSE;

	return TRUE;;
}


char* mapOptoString(int op){
	char *opStr;

	opStr = malloc(sizeof(char) * 7);

	if(op == OPT_EQ)
		strcpy(opStr, "=");
	else if(op == OPT_NEQ)
		strcpy(opStr, "!=");
	else if(op == OPT_LEQ)
		strcpy(opStr, "<=");
	else if(op == OPT_GEQ)
		strcpy(opStr, ">=");
	else if(op == OPT_LGEQ)
		strcpy(opStr, "[,]");
	else if(op == OPT_IN)
		strcpy(opStr, "\\in");
	else if(op == OPT_NOTIN)
		strcpy(opStr, "\\notin");
	else
		strcpy(opStr, "NULL");

	return opStr;
}


char* mapMatchingTypetoString(int function){
	char *opStr;

	opStr = malloc(sizeof(char) * 500);

	if(function == MATCHING_TYPE_STABBING_SUBSCRIPTION_EQUALITY_ONLY_EVENT)
		strcpy(opStr, "MATCHING_TYPE_STABBING_SUBSCRIPTION_EQUALITY_ONLY_EVENT");
	else if(function == MATCHING_TYPE_STABBING_SUBSCRIPTION_EXPRESSIVE_EVENT)
		strcpy(opStr, "MATCHING_TYPE_STABBING_SUBSCRIPTION_EXPRESSIVE_EVENT");
	else if(function == MATCHING_TYPE_SYMMETRIC_MATCHING_EXPRESSIVE_EVENT)
		strcpy(opStr, "MATCHING_TYPE_SYMMETRIC_MATCHING_EXPRESSIVE_EVENT");
	else
		strcpy(opStr, "NULL");

	return opStr;
}



char* mapHashFunctoString(int hashFunc){
	char *opStr;

	opStr = malloc(sizeof(char) * 500);

	if(hashFunc == HASH_FUNC_NAIVE)
		strcpy(opStr, "HASH_FUNC_NAIVE");
	else if(hashFunc == HASH_FUNC_ROBERT_JENKINS)
		strcpy(opStr, "HASH_FUNC_ROBERT_JENKINS");
	else if(hashFunc == HASH_FUNC_THOMAS_WONG)
		strcpy(opStr, "HASH_FUNC_THOMAS_WONG");
	else
		strcpy(opStr, "NULL");

	return opStr;
}




inline static int get_operator_rank(const RPredicate *pred){
	if(pred->operator == OPT_EQ)
		return RANK_OPT_EQ;
	else if(pred->operator == OPT_NEQ)
		return RANK_OPT_NEQ;
	else if(pred->operator == OPT_LEQ)
		return RANK_OPT_LEQ;
	else if(pred->operator == OPT_GEQ)
		return RANK_OPT_GEQ;
	else if(pred->operator == OPT_LGEQ)
		return RANK_OPT_LGEQ;
	else if(pred->operator == OPT_IN)
		return RANK_OPT_IN;
	else if(pred->operator == OPT_NOTIN)
		return RANK_OPT_NOTIN;
	else
		fatal_error("Invalid operator!");

	return -1;
}


inline static int compare_rank_operator(const RPredicate *pred1, const RPredicate *pred2){
	if(get_operator_rank(pred1) == get_operator_rank(pred2))
		return 0;
	else if(get_operator_rank(pred1) > get_operator_rank(pred2))
		return 1;

	return -1;
}

/*
int compare_boundary_size_rpredicate(const void *key1, const void *key2){
	const RPredicate *pred1, *pred2;
	pred1 = key1;
	pred2 = key2;

	if(getBoundaryArea(&pred1->valuesBoundary) < getBoundaryArea(&pred2->valuesBoundary))
		return -1;
	else if(getBoundaryArea(&pred1->valuesBoundary) > getBoundaryArea(&pred2->valuesBoundary))
		return 1;

	return 0;
}
*/

int compare_eqrank_rpredicate(const void *key1, const void *key2){
	const RPredicate *pred1, *pred2;
	pred1 = key1;
	pred2 = key2;

	if(pred1->operator == OPT_EQ){
		if(pred2->operator == OPT_EQ){
			return 0;
		}
		return -1;
	}else if(pred2->operator == OPT_EQ){
		return 1;
	}else if(pred1->valuesBoundary.minBoundary == pred1->valuesBoundary.maxBoundary){
		if(pred2->valuesBoundary.minBoundary == pred2->valuesBoundary.maxBoundary){
			return 0;
		}
		return -1;
	}else if(pred2->valuesBoundary.minBoundary == pred2->valuesBoundary.maxBoundary){
		return 1;
	}else if((pred1->valuesBoundary.maxBoundary - pred1->valuesBoundary.minBoundary) > (pred2->valuesBoundary.maxBoundary - pred2->valuesBoundary.minBoundary) ){
		return 1;
	}else if((pred1->valuesBoundary.maxBoundary - pred1->valuesBoundary.minBoundary) < (pred2->valuesBoundary.maxBoundary - pred2->valuesBoundary.minBoundary) ){
		return -1;
	}else if((pred1->valuesBoundary.maxBoundary - pred1->valuesBoundary.minBoundary) == (pred2->valuesBoundary.maxBoundary - pred2->valuesBoundary.minBoundary) ){
		return compare_rank_operator(pred1, pred2);
	}else{
		fatal_error("compare_eqrank_rpredicate failed!");
	}

	return 0;
}


static void print_object_helper(Object *currSub, Object *currEvent, int isNewLine){
	int i, j, currK;
	RPredicate *currPred;
	PredScore currSubScore;

	currK = 0;
	for(j = 0; j < currSub->arraySize; j++){
		if(currSub->predicate[j]->operator != OPT_NEQ && currSub->predicate[j]->operator != OPT_NOTIN){
			currK++;
		}
	}

	if(currEvent != NULL){
		currSubScore = 0;
		for(j = 0; j < currSub->arraySize; j++ ){
			for(i = 0; i < currEvent->arraySize; i++ ){
				if(currEvent->predicate[i]->attribute == currSub->predicate[j]->attribute){
					currSubScore += currSub->score[j] * currEvent->score[i];
					break;
				}
			}

		}
	}

	if(currEvent != NULL){
		printf("Sub id: %d size (%d, %d, %d) \t [[ score %.4f ]] --> ", currSub->id, currSub->arraySize, currK, currSub->numEqualityLeft, currSubScore);
	}else{
		printf("Sub id: %d size (%d, %d, %d) --> ", currSub->id, currSub->arraySize, currK, currSub->numEqualityLeft);
	}

	for(j = 0; j < currSub->arraySize; j++ ){
		currPred = currSub->predicate[j];

		if(isNewLine) printf("\n");
		printf("Pred addr: %ld \t id: %d \t weight %.4f \t active: %d \t attr: %d \t op: (%s) \t ", (long int)currPred, currPred->id, currSub->score[j], currSub->active[j], currPred->attribute, mapOptoString(currPred->operator));
		printf("\t v: (");
		for(i = 0; i < currSub->predicate[j]->valuesArraySize; i++ ){
			printf("%d,", currPred->values[i]);
		}
		printf(")");
		printf("\t s: %d \t range [%d, %d];\t", currPred->valuesArraySize, currPred->valuesBoundary.minBoundary, currPred->valuesBoundary.maxBoundary);
	}
	puts("");
}

void print_single_object_with_score(Object *currSub, Object *currEvent){
	print_object_helper(currSub, currEvent, TRUE);
}

void print_single_object(Object *currSub){
	print_object_helper(currSub, NULL, TRUE);
}

void print_object(Object *currSub){
	print_object_helper(currSub, NULL, FALSE);
}

void print_object_with_score(Object *currSub, Object *currEvent){
	print_object_helper(currSub, currEvent, FALSE);
}



static void print_objectStream_helper(ObjectStream *currSub, int isNewLine){
	int i, j, currK;
	RPredicateStream *currPred;

	currK = 0;
	for(j = 0; j < currSub->arraySize; j++){
		if(currSub->predicate[j]->operator != OPT_NEQ && currSub->predicate[j]->operator != OPT_NOTIN){
			currK++;
		}
	}

	printf("Sub id: %d size (%d, %d, %d) --> ", currSub->id, currSub->arraySize, currK, currSub->numEqualityLeft);
	for(j = 0; j < currSub->arraySize; j++ ){
		currPred = currSub->predicate[j];

		if(isNewLine) printf("\n");
		printf("Pred addr: %ld \t id: %d \t active: %d \t attr: %d \t op: (%s) \t ", (long int)currPred, currPred->id, currSub->active[j], currPred->attribute, mapOptoString(currPred->operator));
		printf("\t v: (");
		for(i = 0; i < currSub->predicate[j]->valuesArraySize; i++ ){
			printf("%d,", currPred->values[i]);
		}
		printf(")");
		printf("\t s: %d \t range [%d, %d] ", currPred->valuesArraySize, currPred->valuesBoundary.minBoundary, currPred->valuesBoundary.maxBoundary);
	}
	puts("");
}


void print_single_object_stream(ObjectStream *currSub){
	print_objectStream_helper(currSub, TRUE);
}

void print_object_stream(ObjectStream *currSub){
	print_objectStream_helper(currSub, FALSE);
}

