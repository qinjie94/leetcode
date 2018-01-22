/*
 * experiment.c
 *
 *  Created on: May 21, 2009
 *      Author: Mo Sadoghi
 */

#include "workload.h"



char* to_string_ptr_subscription(const void*);
char* to_string_ptr_predicate(const void*);


/* fun_create_workload_function_generics_cluster(ClusterPropagation) */


static DEFINE_HASHTABLE_INSERT(insert_predicate, RPredicate, RPredicate);
static DEFINE_HASHTABLE_SEARCH(search_predicate, RPredicate, RPredicate);


static DEFINE_HASHTABLE_INSERT(insert_dim, char, int);
static DEFINE_HASHTABLE_SEARCH(search_dim, char, int);


static DEFINE_HASHTABLE_INSERT(insert_str, char, int);
static DEFINE_HASHTABLE_SEARCH(search_str, char, int);


static DEFINE_HASHTABLE_INSERT(insert_scramble_obj_list, uint32_t, Object);
static DEFINE_HASHTABLE_SEARCH(search_scramble_obj_list, uint32_t, Object);


static DEFINE_HASHTABLE_INSERT(insert_scramble_pred_list, uint32_t, RPredicateScorePair);
static DEFINE_HASHTABLE_SEARCH(search_scramble_pred_list, uint32_t, RPredicateScorePair);


static DEFINE_HASHTABLE_INSERT(insert_frequency, AttrValuePair, PredScore);
static DEFINE_HASHTABLE_SEARCH(search_frequency, AttrValuePair, PredScore);


List random_sub_uniform(const ExperimentParameter* experiment, WorkloadStat* workloadStat, int isInforceSubDim){
	Sub *s;
	Predicate* p;
	int i, j;
	HashTable tmp;
	int* k;
	List subList = make_empty_LIST( NULL, free );
	int subDimCount;

	if(experiment->systemDim > MAX_NUMBER_OF_DIMENSION)
		fatal_error("The number of requested number of dimensions exceeds the maximum number dimensions allowed by the system!");


	subDimCount = 0;
	for(i = 0; i < experiment->subCount; i++ ){
		s = malloc(sizeof(Sub));
		s->id = i;
		s->predicate = make_empty_LIST( NULL, free );
		s->subPredicate = create_HASH( experiment->subMaxDim );
		tmp = create_HASH( experiment->subMaxDim );
		for(j = 0; j < experiment->subMaxDim; j++ ){
			k = malloc(sizeof(int));
			p = malloc(sizeof(Predicate));

			p->id = equilikely(1, experiment->systemDim-1)*(MAX_NUMBER_OF_DIMENSION);

			*k = p->id;
			if(find_HASH(k, tmp, hash_key_ptr_int, compare_ptr_int) == NULL){
				insert_HASH(k, k, tmp, hash_key_ptr_int, compare_ptr_int);

				p->id += equilikely(1, experiment->systemDistinctValue-1);

				insert_last_LIST(p, s->predicate);
				insert_HASH(p, p, s->subPredicate, hash_key_ptr_predicate, compare_ptr_predicate);
				subDimCount++;
			}else{
				if(isInforceSubDim) j--;
				free(k);
				free(p);
			}
		}

		destroy_HASH(tmp, free, dummy_free);
		insert_last_LIST(s, subList);
	}

	workloadStat->avgObjectDim = (float)subDimCount/(float)experiment->subCount;

	return subList;
}


List random_zsub_uniform(const ExperimentParameter* experiment, WorkloadStat* workloadStat){
	ZSub *s;
	ZPredicate* p;
	int i, j;
	HashTable tmp;
	int* k;
	List subList = make_empty_LIST( NULL, free );
	int subDimCount;

	if(experiment->systemDim > MAX_NUMBER_OF_DIMENSION)
		fatal_error("The number of requested number of dimensions exceeds the maximum number dimensions allowed by the system!");

	subDimCount = 0;
	for(i = 0; i < experiment->subCount; i++ ){
		s = malloc(sizeof(ZSub));
		s->id = i;
		s->predicate = make_empty_LIST( NULL, free );
		tmp = create_HASH( experiment->subMaxDim );
		for(j = 0; j < experiment->subMaxDim; j++ ){
			k = malloc(sizeof(int));
			p = malloc(sizeof(ZPredicate));

			/* Favour the first few dim */
			//if( equilikely(1, 4) == 2){
			//	p->id = equilikely(1, experiment->systemDim/2-1);
			//}else{
				p->id = equilikely(1, experiment->systemDim);
				equilikely(1, 1); // DUMMY seed to match advance matching technique
			//}

			*k = p->id;
			if(find_HASH(k, tmp, hash_key_ptr_int, compare_ptr_int) == NULL){
				insert_HASH(k, k, tmp, hash_key_ptr_int, compare_ptr_int);

				/* Favour the first few dim value */
				//if(p->id < experiment->systemDim /2)
				//	p->value = equilikely(1, 3);
				//else
					p->value = equilikely(1, experiment->systemDistinctValue);

				insert_last_LIST(p, s->predicate);
				subDimCount++;
			}else{
				if(experiment->isInforceSubDim) j--;
				free(k);
				free(p);
			}

		}

		destroy_HASH(tmp, free, dummy_free);
		insert_last_LIST(s, subList);
	}

	workloadStat->avgObjectDim = (float)subDimCount/(float)experiment->subCount;

	return subList;
}


static uint32_t equilikely_inclusive(int lowerBound, int upperBound){
	return get_min_uint32_t(equilikely(lowerBound, upperBound + 1), upperBound);
}



static void countEqualityTypePredicate(const WorkloadParameter* workload, Object *currObj){
	RPredicate *currPred;
	int i, j, currK;
	int isEquality;

	isEquality = 0;
	for(i = 0; i < currObj->arraySize; i++){
		if(currObj->predicate[i]->valuesBoundary.minBoundary == currObj->predicate[i]->valuesBoundary.maxBoundary)
			isEquality++;
		else{
			break;
		}
	}

	for(; i < currObj->arraySize; i++){
		if(currObj->predicate[i]->valuesBoundary.minBoundary == currObj->predicate[i]->valuesBoundary.maxBoundary){
			currK = 0;
			for(j = 0; j < currObj->arraySize; j++){
				if(currObj->predicate[j]->operator != OPT_NEQ && currObj->predicate[j]->operator != OPT_NOTIN){
					currK++;
				}
			}

			printf("Sub id: %d size (%d, %d) --> ", currObj->id, currObj->arraySize, currK);
			for(j = 0; j < currObj->arraySize; j++ ){
				currPred = currObj->predicate[j];

				printf("Pred addr: %ld \t id: %d \t attr: %d \t op: (%s) \t ", (long int)currPred, currPred->id, currPred->attribute, mapOptoString(currPred->operator));
				printf("\t v: (");
				for(i = 0; i < currObj->predicate[j]->valuesArraySize; i++ ){
					printf("%d,", currPred->values[i]);
				}
				printf(")");
				printf("\t s: %d \t range [%d, %d] ", currPred->valuesArraySize, currPred->valuesBoundary.minBoundary, currPred->valuesBoundary.maxBoundary);
			}
			puts("");

			fatal_error("ohh");
		}

	}



	if(workload->isInforceOeq && currObj->numEqualityLeft == 0){
		print_single_object(currObj);
		fatal_error("Num of equality is 0!");
	}

	if(isEquality != currObj->numEqualityLeft){
		print_single_object(currObj);
		fatal_error("Number of equality mismatch!");
	}
}


static int isEQObject(Object *currObject){
	int i;

	for(i = 0; i < currObject->arraySize; i++){
		if(currObject->predicate[i]->operator != OPT_EQ){
			return FALSE;
		}
	}

	return TRUE;
}


static int countEQObject(Object *currObject){
	int i;
	int count;

	count = 0;
	for(i = 0; i < currObject->arraySize; i++){
		if(currObject->predicate[i]->operator == OPT_EQ){
			count++;
		}
	}

	return count;
}


static void minExtendedSize(RPredicate *newPred, int currMaxDistinctValue){
	if(newPred->values[1] - newPred->values[0] == 0){
		if(newPred->values[1] < currMaxDistinctValue)
			newPred->values[1]++;
		else if(newPred->values[0] > 1)
			newPred->values[0]--;
		else
			fatal_error("Cannot extended the object to minExtendedSize");
	}
}


void compute_workload_attribute_frequency(List objList, PredScore *idfIndex){
	ListPosition objectPos;
	Object *currObject;
	RPredicate *currPred;
	int predCounter;

	objectPos = header_LIST( objList );

	/* Iterate through each object and write them to the file */
	while((objectPos = advance_LIST(objectPos)) != NULL){
		currObject = retrieve_LIST(objectPos);

		for(predCounter = 0; predCounter < currObject->arraySize; predCounter++){
			currPred = currObject->predicate[predCounter];
			idfIndex[currPred->attribute]++;
		}
	}
}


void compute_workload_attribute_frequency_hash(List objList, struct hashtable *idfHashIndex, int scoringType, int isControlledMatching){
	ListPosition objectPos;
	Object *currObject;
	RPredicate *currPred;
	int predCounter;
	AttrValuePair *currAttrValuePair;
	PredScore *attrValuePairScore, *tmpAttrValuePairScore;


	objectPos = header_LIST( objList );

	/* Iterate through each object and write them to the file */
	while((objectPos = advance_LIST(objectPos)) != NULL){
		currObject = retrieve_LIST(objectPos);

		for(predCounter = 0; predCounter < currObject->arraySize; predCounter++){
			currPred = currObject->predicate[predCounter];
			currAttrValuePair = malloc(sizeof(AttrValuePair));
			attrValuePairScore = malloc(sizeof(PredScore));

			if(isControlledMatching){
				assert(currPred->baseRPredicate->operator == OPT_EQ);
			}else{
				assert(currPred == currPred->baseRPredicate);
			}

			currAttrValuePair->attribute = currPred->baseRPredicate->attribute;

			if(scoringType == SCORING_ATTRIBUTE_VALUE_FREQUENCY)
				currAttrValuePair->value = currPred->baseRPredicate->values[0];
			else if(scoringType == SCORING_ATTRIBUTE_FREQUENCY)
				currAttrValuePair->value = 0;
			else
				fatal_error("Invalid Scoring type!");

			*attrValuePairScore = 1;

			if((tmpAttrValuePairScore = search_frequency(idfHashIndex, currAttrValuePair)) == NULL){
				if (!insert_frequency(idfHashIndex, currAttrValuePair, attrValuePairScore)) fatal_error("Cannot insert into idf Index!");
			}else{
				++(*tmpAttrValuePairScore);
				free(currAttrValuePair);
				free(attrValuePairScore);
			}
		}
	}
}



void compute_idf_score(PredScore *idfIndex, int dimCount, int numberOfObject){
	int i = 0;
	PredScore highestScore = 0;

	for(i = 0; i < dimCount; i++){
		if(idfIndex[i] != 0){
			idfIndex[i] = numberOfObject / idfIndex[i];
			if(highestScore < idfIndex[i]) highestScore = idfIndex[i];
		}
	}

	for(i = 0; i < dimCount; i++){
		if(idfIndex[i] != 0){
			idfIndex[i] = idfIndex[i] / highestScore;
		}
	}
}


void compute_idf_score_hash(struct hashtable *idfHashIndex, int numberOfObject){
	PredScore highestScore = 0;
	PredScore *currAttrValuePairScore;

	struct hashtable_itr *idfIndexIterator;

	/* Find the highest value for normalization */
	idfIndexIterator = hashtable_iterator(idfHashIndex);
	if (hashtable_count(idfHashIndex) > 0){
		do {
			currAttrValuePairScore = hashtable_iterator_value(idfIndexIterator);
			*currAttrValuePairScore = numberOfObject / *currAttrValuePairScore;

			if(highestScore < *currAttrValuePairScore) highestScore = *currAttrValuePairScore;

		} while (hashtable_iterator_advance(idfIndexIterator));
	}
	free(idfIndexIterator);
	idfIndexIterator = NULL;

	/* Normalize the values */
	idfIndexIterator = hashtable_iterator(idfHashIndex);
	if (hashtable_count(idfHashIndex) > 0){
		do {
			currAttrValuePairScore = hashtable_iterator_value(idfIndexIterator);
			*currAttrValuePairScore = *currAttrValuePairScore / highestScore;
		} while (hashtable_iterator_advance(idfIndexIterator));
	}
	free(idfIndexIterator);
	idfIndexIterator = NULL;
}



/* Generate the score for each attribute using a normal distributions */
void compute_object_pred_score(List objectList, const WorkloadParameter* workload){
	ListPosition objectPos;
	Object *currObject;
	RPredicate *currPred;
	int predCounter;

	objectPos = header_LIST( objectList );

	/* Iterate through each object and write them to the file */
	while((objectPos = advance_LIST(objectPos)) != NULL){
		currObject = retrieve_LIST(objectPos);

		for(predCounter = 0; predCounter < currObject->arraySize; predCounter++){
			currPred = currObject->predicate[predCounter];
			assert(workload->idfIndex[currPred->attribute] != 0);
			currObject->score[predCounter] = get_min_predscore(normal(workload->systemDimMeanScoreRatio * workload->idfIndex[currPred->attribute], sqrt(workload->systemDimVarianceScoreRatio) * workload->idfIndex[currPred->attribute]), workload->idfIndex[currPred->attribute]);
		}

	}

}


/* Generate the score for each attribute using a normal distributions */
void compute_object_pred_score_hash(List objectList, const WorkloadParameter* workload, int scoringType){
	ListPosition objectPos;
	Object *currObject;
	RPredicate *currPred;
	int predCounter;
	PredScore *currPredScore;
	AttrValuePair currAttrValuePair;

	objectPos = header_LIST( objectList );

	/* Iterate through each object and write them to the file */
	while((objectPos = advance_LIST(objectPos)) != NULL){
		currObject = retrieve_LIST(objectPos);

		for(predCounter = 0; predCounter < currObject->arraySize; predCounter++){
			currPred = currObject->predicate[predCounter];
			currAttrValuePair.attribute = currPred->baseRPredicate->attribute;

			if(scoringType == SCORING_ATTRIBUTE_VALUE_FREQUENCY)
				currAttrValuePair.value = currPred->baseRPredicate->values[0];
			else if(scoringType == SCORING_ATTRIBUTE_FREQUENCY)
				currAttrValuePair.value = 0;
			else
				fatal_error("Invalid Scoring type!");

			if((currPredScore = search_frequency(workload->idfHashIndex, &currAttrValuePair)) != NULL){
				assert(currPredScore != 0);
				currObject->score[predCounter] = get_min_predscore(normal(workload->systemDimMeanScoreRatio * *currPredScore, sqrt(workload->systemDimVarianceScoreRatio) * *currPredScore), *currPredScore);
			}else{
				fatal_error("Missing score!");
			}
		}

	}
}


List random_object_advance(const WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex){
	Object *currObject;
	RPredicate *currPred, *oldPred;
	int i, j, currPredIndex;
	int currDimIndex;
	PredValue currMaxDistinctValue;
	int currMaxDim;
	HashTable tmpHash;
	int* k;
	List objectList = make_empty_LIST( NULL, free );
	int objectDimCount;
	int isAcceptOp, isEqualityChosen, isObjectOeqRatioSet, eqOpCount;
	double remainingOpeq;
	int clusterIndex, clusterValue, clusterSize;
	PredValue maxExtendedObject;

	if(workload->systemDim > MAX_NUMBER_OF_DIMENSION)
		fatal_error("The number of requested number of dimensions exceeds the maximum number dimensions allowed by the system!");

	if(workload->systemMinDistinctValue < MIN_NUMBER_OF_DISTINCT_VALUES_DIMENSION)
		fatal_error("The min number of system distinct value must be at least two!");

	if(workload->objectMaxExtendedObjectSize > workload->systemDistinctValue)
		fatal_error("The number of requested number of distinct value per dimension exceeds the maximum number distinct values allowed by the system!");

	if(workload->objectMaxNumberInNotIn > workload->objectMaxExtendedObjectSize)
		fatal_error("The maximum number of In and NotIn operators must be less than maximum extended object size!");

	if((workload->systemDistinctValueUniform || workload->systemDistinctValueZip) && workload->systemMaxDistinctCluster != 0){
		fatal_error("The each domain must be generated either using uniform or zipf distribution or using clustering!");
	}

	if((workload->objectMaxExtendedObjectSizeUniform || workload->objectMaxExtendedObjectSizeZip) && workload->systemMaxDistinctCluster != 0){
		//error("The each domain must be generated either using uniform or zipf distribution or using clustering; otherwise leads to a very cluster size for k-index!");
	}


	objectDimCount = 0;
	isObjectOeqRatioSet = FALSE;
	remainingOpeq = 0;

	if(workload->objectOeqRatio == 0){
		isObjectOeqRatioSet = FALSE;
	}else{
		isObjectOeqRatioSet = TRUE;
		if(workload->isInforceOeq){
			remainingOpeq = workload->objectOeqRatio - (1/(float)workload->objectMaxDim);
		}else{
			remainingOpeq = workload->objectOeqRatio;
		}
	}

	for(i = 0; i < workload->objectCount; i++ ){
		currMaxDim = equilikely_inclusive(workload->objectMinDim, workload->objectMaxDim);

		currObject = malloc(sizeof(Object));
		currObject->id = i;
		currObject->predicate = malloc(sizeof(RPredicate*) * currMaxDim);
		currObject->active = malloc(sizeof(uint8_t) * currMaxDim);
		currObject->covered = malloc(sizeof(uint8_t) * currMaxDim);
		currObject->score = malloc(sizeof(PredScore) * currMaxDim);
		currObject->arraySize = currMaxDim;
		currObject->numEqualityLeft = 0;

		/* clear the memory for the new object */
		for(j = 0; j < currMaxDim; j++ ){
			currObject->active[j] = FALSE;
			currObject->covered[j] = FALSE;
			currObject->score[j] = 0;
			currObject->predicate[j] = NULL;
		}

		tmpHash = create_HASH( currMaxDim );
		currDimIndex = 0;
		for(j = 0; j < currMaxDim; j++ ){

			k = malloc(sizeof(int));
			currPred = malloc(sizeof(RPredicate));

			/* All predicates generated in this method are implicitly base predicate.
			 * To indicate that a predicate is a base predicate, we point it to itself.
			 */
			currPred->baseRPredicate = currPred;

			if(workload->objectUniform)
				currPred->attribute = equilikely_inclusive(1, workload->systemDim);
			else if(workload->objectZip)
				currPred->attribute = zipf_first_instance(workload->objectZipAlpha, workload->systemDim);
			else
				fatal_error("Please specify the underlining distribution of the object experiment!");

			if(workload->systemDistinctValueUniform || workload->systemDistinctValueZip){
				if(workload->distinctValueIndex[currPred->attribute] == 0){
					if(workload->systemDistinctValueUniform){
						currMaxDistinctValue = get_max_predvalue_t(equilikely_inclusive(1, workload->systemDistinctValue), workload->systemMinDistinctValue);
					}else{
						currMaxDistinctValue = get_max_predvalue_t(zipf_second_instance(workload->systemDistinctValueZipAlpha, workload->systemDistinctValue), workload->systemMinDistinctValue);
					}
					workload->distinctValueIndex[currPred->attribute] = currMaxDistinctValue;
				}else{
					currMaxDistinctValue = workload->distinctValueIndex[currPred->attribute];
				}
			}else{
				currMaxDistinctValue = workload->systemDistinctValue;
				workload->distinctValueIndex[currPred->attribute] = currMaxDistinctValue;

				if(workload->distinctClusterValueIndex[currPred->attribute] == NULL && workload->systemMaxDistinctCluster != 0){

					if(workload->objectOgeq || workload->objectOleq || workload->objectOin || workload->objectOneq || workload->objectOnotin){
						fatal_error("For cluster domain workload with only objectOeq & objectOlgeq is permitted!");
					}

					if(workload->systemDistinctValue < (2 * workload->systemMaxDistinctClusterSize + 2)){
						fatal_error("systemDistinctValue must be at least twice the size of systemMaxDistinctClusterSize!");
					}
					clusterSize = equilikely_inclusive(1, workload->systemMaxDistinctCluster);

					workload->distinctClusterValueIndex[currPred->attribute] = malloc(sizeof(int) * clusterSize);
					workload->distinctClusterValueIndexSize[currPred->attribute] = clusterSize;

					for(clusterIndex = 0; clusterIndex < clusterSize; clusterIndex++){
						workload->distinctClusterValueIndex[currPred->attribute][clusterIndex] = equilikely_inclusive(workload->systemMaxDistinctClusterSize + 1, currMaxDistinctValue - workload->systemMaxDistinctClusterSize - 1);
					}
				}
			}


			*k = currPred->attribute;
			if(find_HASH(k, tmpHash, hash_key_ptr_int, compare_ptr_int) == NULL){
				insert_HASH(k, k, tmpHash, hash_key_ptr_int, compare_ptr_int);

				isAcceptOp = FALSE;
				isEqualityChosen = TRUE;

				while(!isAcceptOp){
					/* At least one equality */
					if(workload->isInforceOeq && j == 0){
						currPred->operator = OPT_EQ;
					}else{
						if(isObjectOeqRatioSet){
							if(isEqualityChosen && remainingOpeq > uniform(0, 1)){
								/* set to EQ */
								currPred->operator = OPT_EQ;
							}else{
								/* skip EQ */
								currPred->operator = equilikely_inclusive(2, workload->systemObjectOptCount);
								isEqualityChosen = FALSE;
							}
						}else{
							currPred->operator = equilikely_inclusive(1, workload->systemObjectOptCount);
						}
					}

					if(currPred->operator == OPT_EQ && workload->objectOeq)
						isAcceptOp = TRUE;
					else if(currPred->operator == OPT_NEQ && workload->objectOneq)
						isAcceptOp = TRUE;
					else if(currPred->operator == OPT_LEQ && workload->objectOleq)
						isAcceptOp = TRUE;
					else if(currPred->operator == OPT_GEQ && workload->objectOgeq)
						isAcceptOp = TRUE;
					else if(currPred->operator == OPT_LGEQ && workload->objectOlgeq)
						isAcceptOp = TRUE;
					else if(currPred->operator == OPT_IN && workload->objectOin)
						isAcceptOp = TRUE;
					else if(currPred->operator == OPT_NOTIN && workload->objectOnotin)
						isAcceptOp = TRUE;
				}

				currPred->id = currPred->attribute * DIMENSION_SHIFTING_FACTOR + currPred->operator * OPERATORS_SHIFTING_FACTOR;

				if(currPred->operator == OPT_LGEQ){
					//puts("OPT_LGEQ");
					currPred->valuesArraySize = 2;
					currPred->values = malloc(sizeof(PredValue) * currPred->valuesArraySize );

					if(workload->systemMaxDistinctCluster != 0){
						clusterIndex = equilikely_inclusive(1, workload->distinctClusterValueIndexSize[currPred->attribute]) - 1;
						clusterValue = workload->distinctClusterValueIndex[currPred->attribute][clusterIndex];
						currPred->values[0] = equilikely_inclusive(clusterValue - workload->systemMaxDistinctClusterSize/2, clusterValue + workload->systemMaxDistinctClusterSize/2);
					}else{
						currPred->values[0] = equilikely_inclusive(1, currMaxDistinctValue);
					}

					if(workload->objectMaxExtendedObjectSizeUniform){
						maxExtendedObject =  equilikely_inclusive(2, currMaxDistinctValue);
					}else if(workload->objectMaxExtendedObjectSizeZip){
						maxExtendedObject =  zipf_third_instance(workload->objectMaxExtendedObjectSizeZipAlpha, currMaxDistinctValue) + 1;
					}else{
						maxExtendedObject =  workload->objectMaxExtendedObjectSize;
					}

					currPred->values[1] = get_min_predvalue_t(currPred->values[0] + equilikely_inclusive(1, maxExtendedObject), currMaxDistinctValue);

					if(workload->objectHasMinExtendedObjectSize)
						minExtendedSize(currPred, currMaxDistinctValue);

					currPred->id += (currPred->values[0] + currPred->values[1])/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
					setBoundary(&currPred->valuesBoundary, currPred->values[0], currPred->values[1]);

				}else if(currPred->operator == OPT_IN || currPred->operator == OPT_NOTIN){
					currPred->valuesArraySize = equilikely_inclusive(1, workload->objectMaxNumberInNotIn);
					PriorityQueue binPQ;
					int *currValue;
					int startValue = equilikely_inclusive(1, currMaxDistinctValue);
					int endValue = get_min_predvalue_t(startValue + equilikely_inclusive(1, workload->objectMaxExtendedObjectSize), currMaxDistinctValue);

					binPQ = initialize_BINHEAP(currPred->valuesArraySize, get_min_int_value());
					currPred->values = malloc(sizeof(PredValue) * currPred->valuesArraySize );

					for(currPredIndex = 0; currPredIndex < currPred->valuesArraySize; currPredIndex++){
						currValue = malloc( sizeof( int ) );
						*currValue = equilikely_inclusive(startValue, endValue);
						insert_BINHEAP(currValue, currValue, binPQ, compare_ptr_int);
					}

					currPredIndex = 0;
					while (!is_empty_BINHEAP(binPQ)){
						currValue = (int*)delete_min_BINHEAP(binPQ, compare_ptr_int);
						if(currPredIndex == 0){
							currPred->values[currPredIndex] = *currValue;
							currPred->id += currPred->values[currPredIndex]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
							currPredIndex++;
						}else if(currPred->values[currPredIndex-1] != *currValue){
							currPred->values[currPredIndex] = *currValue;
							currPred->id += currPred->values[currPredIndex]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
							currPredIndex++;
						}
						free(currValue);
					}

					if(currPredIndex == 0){
						fatal_error("At least one predicate must be generated for \\in and \\notin operators!");
					}else if(currPredIndex < currPred->valuesArraySize){
						currPred->values = (PredValue*) realloc (currPred->values, currPredIndex * sizeof(PredValue));
						currPred->valuesArraySize = currPredIndex;
					}

					if(currPred->operator == OPT_IN)
						setBoundary(&currPred->valuesBoundary, currPred->values[0], currPred->values[currPred->valuesArraySize - 1]);
					else if(currPred->operator == OPT_NOTIN)
						setBoundary(&currPred->valuesBoundary, 1, currMaxDistinctValue);

					destroy_BINHEAP(binPQ, free, dummy_free);

				}else{
					currPred->valuesArraySize = 1;
					currPred->values = malloc(sizeof(PredValue) * currPred->valuesArraySize );


					if(workload->systemMaxDistinctCluster != 0){
						clusterIndex = equilikely_inclusive(1, workload->distinctClusterValueIndexSize[currPred->attribute]) - 1;
						clusterValue = workload->distinctClusterValueIndex[currPred->attribute][clusterIndex];
						currPred->values[0] = equilikely_inclusive(clusterValue - workload->systemMaxDistinctClusterSize/2, clusterValue + workload->systemMaxDistinctClusterSize/2);
					}else{
						currPred->values[0] = equilikely_inclusive(1, currMaxDistinctValue);
					}
					currPred->id += currPred->values[0]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;

					if(currPred->operator == OPT_EQ){
						setBoundary(&currPred->valuesBoundary, currPred->values[0], currPred->values[0]);
					}else if(currPred->operator == OPT_NEQ)
						setBoundary(&currPred->valuesBoundary, 1, currMaxDistinctValue);
					else if(currPred->operator == OPT_LEQ)
						setBoundary(&currPred->valuesBoundary, 1, currPred->values[0]);
					else if(currPred->operator == OPT_GEQ)
						setBoundary(&currPred->valuesBoundary, currPred->values[0], currMaxDistinctValue);
				}

				if(workload->systemDistinctValueUniform || workload->systemDistinctValueZip)
					assert(workload->distinctValueIndex[currPred->attribute] == currMaxDistinctValue);

				assert(currPred->valuesBoundary.minBoundary >= 0);
				assert(currPred->valuesBoundary.maxBoundary <= currMaxDistinctValue);

				assert(currPred->id > 0);

				if ((oldPred = search_predicate(predicateIndex, currPred)) == NULL){
					if (!insert_predicate(predicateIndex, currPred, currPred)) fatal_error("Cannot insert into predicate into  Predicate Index (hashtable)!");
					currObject->predicate[currDimIndex] = currPred;
				}else{
					free(currPred->values);
					free(currPred);
					currObject->predicate[currDimIndex] = oldPred;
				}

				currObject->active[currDimIndex] = TRUE;
				currObject->covered[currDimIndex] = FALSE;

				if(workload->isEqualityRanking && currPred->valuesBoundary.minBoundary == currPred->valuesBoundary.maxBoundary)
					currObject->numEqualityLeft++;

				objectDimCount++;
				currDimIndex++;
			}else{
				if(workload->isInforceObjectDim){j--;}
				free(k);
				free(currPred);
			}
		}

		if(currDimIndex < currObject->arraySize){
			currObject->active = (uint8_t*) realloc (currObject->active, currDimIndex * sizeof(uint8_t));
			currObject->covered = (uint8_t*) realloc (currObject->covered, currDimIndex * sizeof(uint8_t));
			currObject->score = (PredScore*) realloc (currObject->score, currDimIndex * sizeof(PredScore));
			currObject->predicate = (RPredicate**) realloc (currObject->predicate, currDimIndex * sizeof(RPredicate*));
			currObject->arraySize = currDimIndex;
		}

		if(workload->isEqualityRanking){
			quick_sort((ElementType)currObject->predicate, currObject->arraySize, compare_eqrank_rpredicate);
			countEqualityTypePredicate(workload, currObject);

			if(workload->isInforceOeq && currObject->numEqualityLeft == 0)
				fatal_error("experiment generation is messed up!");

		}

		destroy_HASH(tmpHash, free, dummy_free);
		insert_last_LIST(currObject, objectList);
		if(isEQObject(currObject)) workloadStat->eqObjectCount++;

		eqOpCount = countEQObject(currObject);
		workloadStat->avgEqOpCount += eqOpCount;
		workloadStat->avgNonEqOpCount += currObject->arraySize - eqOpCount;
	}
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = (float)objectDimCount/(float)workload->objectCount;

	return objectList;
}


int test_generate_matching_probability(Object *newObj, Object *oldObj){
	int i, j, k;
	RPredicate *newPred, *oldPred;
	int satisfied;

	for(i = 0; i < newObj->arraySize; i++){
		newPred = newObj->predicate[i];
		for(j = 0; j < oldObj->arraySize; j++){
			oldPred = oldObj->predicate[j];

			if(newPred->attribute == oldPred->attribute){
				if(newPred->operator == OPT_EQ){
					if(newPred->values[0] != oldPred->values[0]){
						printf("newPred->attribute %d, newPred->operator %d \n", newPred->attribute, newPred->operator);
						return FALSE;
					}
				}else if(newPred->operator == OPT_NEQ){
					if(newPred->values[0] == oldPred->values[0]){
						printf("newPred->attribute %d, newPred->operator %d \n", newPred->attribute, newPred->operator);
						return FALSE;
					}
				}else if(newPred->operator == OPT_LEQ){
					if(newPred->values[0] < oldPred->values[0]){
						printf("newPred->attribute %d, newPred->operator %d \n", newPred->attribute, newPred->operator);
						return FALSE;
					}
				}else if(newPred->operator == OPT_GEQ){
					if(newPred->values[0] > oldPred->values[0]){
						printf("newPred->attribute %d, newPred->operator %d \n", newPred->attribute, newPred->operator);
						return FALSE;
					}
				}else if(newPred->operator == OPT_LGEQ){
					if(!(newPred->values[0] <= oldPred->values[0] && newPred->values[1] >= oldPred->values[0])){
						printf("newPred->attribute %d, newPred->operator %d \n", newPred->attribute, newPred->operator);
						return FALSE;
					}
				}else if(newPred->operator == OPT_IN){
					satisfied = FALSE;
					for(k = 0; k < newPred->valuesArraySize; k++){
						if(newPred->values[k] == oldPred->values[0]){
							satisfied = TRUE;
							break;
						}
					}
					if(!satisfied){
						printf("newPred->attribute %d, newPred->operator %d \n", newPred->attribute, newPred->operator);
						return FALSE;
					}
				}else if(newPred->operator == OPT_NOTIN){
					satisfied = TRUE;
					for(k = 0; k < newPred->valuesArraySize; k++){
						if(newPred->values[k] == oldPred->values[0]){
							satisfied = FALSE;
							break;
						}
					}
					if(!satisfied){
						printf("newPred->attribute %d, newPred->operator %d \n", newPred->attribute, newPred->operator);
						return FALSE;
					}
				}
				break;
			}

		}
	}

	return TRUE;
}



void generate_matching_probability(List objList, ExperimentParameter *experimentParameter, WorkloadParameter* workload, WorkloadStat* workloadStat, int duplicationDegree, int minID, struct hashtable *predicateIndex){
	ListPosition objListPos, prevObjListPos;
	Object *currObj, *newObj;
	RPredicate *currPred, *newPred, *oldPred;
	float avgObjDim;
	int eqOpCount;
	int currDuplicationDegree;
	double keepingPredProbability;
	int i, j, newObjIndex;
	int isAcceptOp, isEqualityChosen, hasEqualityDim;
	PredValue currMaxDistinctValue;
	PredValue mustCoveredValue;
	int currPredIndex;
	int minObjDim, maxObjDim;
	uint8_t victimIndex;
	uint16_t victimAttribute;
	PredValue maxExtendedObject;
	int currMinID;

	PredScore *predListScores;
	PredScore totalPredScore;
	double chosenPredScore;
	double accumulatedPredScoreLowerBound, accumulatedPredScoreUpperBound;

	avgObjDim = 0;
	eqOpCount = 0;
	minObjDim = 250;
	maxObjDim = 0;
	currMinID = minID;

	objListPos = header_LIST( objList );
	prevObjListPos = objListPos;

	while((objListPos = advance_LIST(objListPos)) != NULL){
		currObj = retrieve_LIST(objListPos);


		currDuplicationDegree = duplicationDegree;

		if(currObj->arraySize > maxObjDim)
			maxObjDim = currObj->arraySize;

		if(currObj->arraySize < minObjDim)
			minObjDim = currObj->arraySize;


		if(workload->objectIsIncludeBaseTupe){
			eqOpCount = currObj->arraySize;
			workloadStat->avgEqOpCount += eqOpCount;
			workloadStat->avgNonEqOpCount += currObj->arraySize - eqOpCount;
			avgObjDim += currObj->arraySize;
		}else{
			currDuplicationDegree++;
		}

		while(currDuplicationDegree--){

			newObj = clone_object(currObj);
			newObj->id = ++currMinID;
			newObj->arraySize = 0;
			newObj->numEqualityLeft = 0;

			if(duplicationDegree != 0)
				keepingPredProbability = (double) equilikely_inclusive(workload->objectMinDim, workload->objectMaxDim) / (double)currObj->arraySize;
			else
				keepingPredProbability = 1;

			newObjIndex = 0;
			hasEqualityDim = FALSE;

			if(experimentParameter->dataDistributionType == DERIVED_OBJECTS_SELECTED_BASED_ON_ORIGINAL_DISTRIBUTIONS){
				predListScores = malloc(sizeof(PredScore) * currObj->arraySize);
				totalPredScore = 0;

				/* Extract the true score for each predicate in the events to be used for generation of the derived predicates
				 * For now we are only considering (attr) frequency because the derived predicate could span the entire space.
				 *
				 */
				for(j = 0; j < currObj->arraySize; j++){
					assert(currObj->predicate[j] == currObj->predicate[j]->baseRPredicate);
					predListScores[j] = experimentParameter->idfIndex[currObj->predicate[j]->baseRPredicate->attribute];
					totalPredScore += predListScores[j];
				}

				/* Normalize the score */
				for(j = 0; j < currObj->arraySize; j++){
					predListScores[j] = predListScores[j]/totalPredScore;
				}
			}

			while(newObjIndex == 0){
				for(i = 0; i < currObj->arraySize; i++){

					if(keepingPredProbability > uniform(0, 1)){

						/*
						 * Depending on workload distribution type, we either choose the current predicate as our predicate,
						 * or we choose a predicate from the list of current available predicates (derived from the base object).
						 */
						if(experimentParameter->dataDistributionType == DERIVED_OBJECTS_SELECTED_BASED_ON_ORIGINAL_DISTRIBUTIONS){

							/* Pick score */
							chosenPredScore = uniform(0, 1);

							/* Now find under which predicate range of score the randomly chosen pred score has fallen.
							 * Pick the corresponding predicate as the next 'currPred' and set its score to zero to prevent
							 * from being selected again.
							 */
							accumulatedPredScoreLowerBound = 0;
							accumulatedPredScoreUpperBound = 0;
							for(j = 0; j < currObj->arraySize; j++){
								accumulatedPredScoreUpperBound += predListScores[j];

								if(predListScores[j] != 0 && chosenPredScore >= accumulatedPredScoreLowerBound && chosenPredScore <= accumulatedPredScoreUpperBound){
									currPred = currObj->predicate[j];
									predListScores[j] = 0;
									break;
								}

								accumulatedPredScoreLowerBound += predListScores[j];
							}

							/* Recompute the total score followed by normalizing them again */
							totalPredScore = 0;
							for(j = 0; j < currObj->arraySize; j++){
								totalPredScore += predListScores[j];
							}

							for(j = 0; j < currObj->arraySize; j++){
								if(totalPredScore != 0) predListScores[j] = predListScores[j]/totalPredScore;
							}

						}else{
							currPred = currObj->predicate[i];
						}

						newPred = malloc(sizeof(RPredicate));
						newPred->attribute = currPred->attribute;
						currMaxDistinctValue = workload->distinctValueIndex[newPred->attribute];
						mustCoveredValue = currPred->values[0];

						/* All predicates generated in this method are implicitly derived predicate.
						 * To indicate that a predicate is a derived predicate, we must point its base predicate.
						 * Furthermore, we allow that base predicate itself derived from another predicate; thus,
						 * we support at most two layers of basing.
						 */
						newPred->baseRPredicate = currPred->baseRPredicate;


						isAcceptOp = FALSE;
						/* isEqualityChosen flag is give fair chance to non-equality operator.
						 * For instance, if non-equality operator is chosen, but the actually
						 * operator is not acceptable, then we must continue selecting non-equality
						 * until a right candidate is chosen, and we must prevent OP_EQ within this
						 * current transaction.
						 */
						isEqualityChosen = TRUE;

						while(!isAcceptOp){
							/* At least one equality */
								if(isEqualityChosen && workload->objectOeqRatio > uniform(0, 1)){
									/* set to EQ */
									newPred->operator = OPT_EQ;
									hasEqualityDim = TRUE;
								}else{
									/* skip EQ */
									newPred->operator = equilikely_inclusive(2, workload->systemObjectOptCount);
									isEqualityChosen = FALSE;
								}

							if(newPred->operator == OPT_EQ && workload->objectOeq)
								isAcceptOp = TRUE;
							else if(newPred->operator == OPT_NEQ && workload->objectOneq)
								isAcceptOp = TRUE;
							else if(newPred->operator == OPT_LEQ && workload->objectOleq)
								isAcceptOp = TRUE;
							else if(newPred->operator == OPT_GEQ && workload->objectOgeq)
								isAcceptOp = TRUE;
							else if(newPred->operator == OPT_LGEQ && workload->objectOlgeq)
								isAcceptOp = TRUE;
							else if(newPred->operator == OPT_IN && workload->objectOin)
								isAcceptOp = TRUE;
							else if(newPred->operator == OPT_NOTIN && workload->objectOnotin)
								isAcceptOp = TRUE;
						}

						newPred->id = newPred->attribute * DIMENSION_SHIFTING_FACTOR + newPred->operator * OPERATORS_SHIFTING_FACTOR;

						if(newPred->operator == OPT_LGEQ){
							newPred->valuesArraySize = 2;
							newPred->values = malloc(sizeof(PredValue) * newPred->valuesArraySize );

							if(workload->objectMaxExtendedObjectSizeUniform){
								maxExtendedObject =  equilikely_inclusive(2, currMaxDistinctValue);
							}else if(workload->objectMaxExtendedObjectSizeZip){
								maxExtendedObject =  zipf_third_instance(workload->objectMaxExtendedObjectSizeZipAlpha, currMaxDistinctValue) + 1;
							}else{
								maxExtendedObject =  workload->objectMaxExtendedObjectSize;
							}

							newPred->values[0] = equilikely_inclusive(get_max_predvalue_t(1, get_positive_predvalue_t(mustCoveredValue - maxExtendedObject)), mustCoveredValue);
							newPred->values[1] = get_max_predvalue_t(mustCoveredValue, get_min_predvalue_t(newPred->values[0] + equilikely_inclusive(1, maxExtendedObject), currMaxDistinctValue));


							if(workload->objectHasMinExtendedObjectSize)
								minExtendedSize(newPred, currMaxDistinctValue);

							newPred->id += (newPred->values[0] + newPred->values[1])/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
							setBoundary(&newPred->valuesBoundary, newPred->values[0], newPred->values[1]);

						}else if(newPred->operator == OPT_IN || newPred->operator == OPT_NOTIN){
							newPred->valuesArraySize = equilikely_inclusive(1, workload->objectMaxNumberInNotIn);
							PriorityQueue binPQ;
							int *currValue;
							int startValue, endValue;

							if(newPred->operator == OPT_IN){
								startValue = equilikely_inclusive(get_max_predvalue_t(1, get_positive_predvalue_t(mustCoveredValue - workload->objectMaxExtendedObjectSize)), mustCoveredValue);
								endValue = get_max_predvalue_t(mustCoveredValue, get_min_predvalue_t(startValue + equilikely_inclusive(1, workload->objectMaxExtendedObjectSize), currMaxDistinctValue));
							}else if(newPred->operator == OPT_NOTIN){
								startValue = 1;
								endValue = currMaxDistinctValue;
							}

							binPQ = initialize_BINHEAP(newPred->valuesArraySize, get_min_int_value());
							newPred->values = malloc(sizeof(PredValue) * newPred->valuesArraySize );

							for(currPredIndex = 0; currPredIndex < newPred->valuesArraySize; currPredIndex++){
								currValue = malloc( sizeof( int ) );
								if(currPredIndex == 0 && newPred->operator == OPT_IN){
									*currValue = mustCoveredValue;
								}else{
									do{
										*currValue = equilikely_inclusive(startValue, endValue);
									}while(newPred->operator == OPT_NOTIN && *currValue == mustCoveredValue);
								}
								insert_BINHEAP(currValue, currValue, binPQ, compare_ptr_int);
							}

							currPredIndex = 0;
							while (!is_empty_BINHEAP(binPQ)){
								currValue = (int*)delete_min_BINHEAP(binPQ, compare_ptr_int);
								if(currPredIndex == 0){
									newPred->values[currPredIndex] = *currValue;
									newPred->id += newPred->values[currPredIndex]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
									currPredIndex++;
								}else if(newPred->values[currPredIndex-1] != *currValue){
									newPred->values[currPredIndex] = *currValue;
									newPred->id += newPred->values[currPredIndex]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
									currPredIndex++;
								}
								free(currValue);
							}

							if(currPredIndex == 0){
								fatal_error("At least one predicate must have beeen generated for \\in and \\notin operators!");
							}else if(currPredIndex < newPred->valuesArraySize){
								newPred->values = (PredValue*) realloc (newPred->values, currPredIndex * sizeof(PredValue));
								newPred->valuesArraySize = currPredIndex;
							}

							if(newPred->operator == OPT_IN)
								setBoundary(&newPred->valuesBoundary, newPred->values[0], newPred->values[newPred->valuesArraySize - 1]);
							else if(newPred->operator == OPT_NOTIN)
								setBoundary(&newPred->valuesBoundary, 1, currMaxDistinctValue);

							destroy_BINHEAP(binPQ, free, dummy_free);

						}else{
							newPred->valuesArraySize = 1;
							newPred->values = malloc(sizeof(PredValue) * newPred->valuesArraySize );

							if(newPred->operator == OPT_EQ){
								newPred->values[0] = mustCoveredValue;
								setBoundary(&newPred->valuesBoundary, newPred->values[0], newPred->values[0]);
							}else if(newPred->operator == OPT_NEQ){

								do{
									newPred->values[0] = equilikely_inclusive(1, currMaxDistinctValue);
								}while(newPred->values[0] == mustCoveredValue);

								setBoundary(&newPred->valuesBoundary, 1, currMaxDistinctValue);
							}else if(newPred->operator == OPT_LEQ){
								newPred->values[0] = equilikely_inclusive(mustCoveredValue, currMaxDistinctValue);
								setBoundary(&newPred->valuesBoundary, 1, newPred->values[0]);
							}else if(newPred->operator == OPT_GEQ){
								newPred->values[0] = equilikely_inclusive(1, mustCoveredValue);
								setBoundary(&newPred->valuesBoundary, newPred->values[0], currMaxDistinctValue);
							}

							newPred->id += newPred->values[0]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;

						}

						assert(currPred->id > 0);

						if ((oldPred = search_predicate(predicateIndex, newPred)) == NULL){
							if (!insert_predicate(predicateIndex, newPred, newPred)) fatal_error("Cannot insert into predicate into  Predicate Index (hashtable)!");
							newObj->predicate[newObjIndex] = newPred;
						}else{
							free(newPred->values);
							free(newPred);
							newObj->predicate[newObjIndex] = oldPred;
						}

						newObj->active[newObjIndex] = TRUE;
						newObj->covered[newObjIndex] = FALSE;

						if(workload->isEqualityRanking && newPred->valuesBoundary.minBoundary == newPred->valuesBoundary.maxBoundary)
							newObj->numEqualityLeft++;

						newObjIndex++;
					}
				}
			}


			/* if no equality is chosen, then randomly pick predicate in newObj and use the original predicate in currObj instead of it */
			if(workload->isInforceOeq && !hasEqualityDim){
				if(newObjIndex == 0){
					victimIndex = equilikely(0, currObj->arraySize);
					newObj->predicate[0] = currObj->predicate[victimIndex];
					newObj->numEqualityLeft++;
					newObjIndex++;
				}else{
					victimIndex = equilikely(0, newObjIndex);
					victimAttribute = newObj->predicate[victimIndex]->attribute;

					for(i = 0; i < currObj->arraySize; i++){
						currPred = currObj->predicate[i];

						if(currPred->attribute == victimAttribute){
							if(newObj->predicate[victimIndex]->valuesBoundary.minBoundary != newObj->predicate[victimIndex]->valuesBoundary.maxBoundary)
								newObj->numEqualityLeft++;

							newObj->predicate[victimIndex] = currPred;
							break;
						}

					}
				}
			}

			newObj->arraySize = newObjIndex;

			if(newObjIndex < currObj->arraySize){
				newObj->active = (uint8_t*) realloc (newObj->active, newObj->arraySize * sizeof(uint8_t));
				newObj->covered = (uint8_t*) realloc (newObj->covered, newObj->arraySize * sizeof(uint8_t));
				newObj->score = (PredScore*) realloc (newObj->score, newObj->arraySize * sizeof(PredScore));
				newObj->predicate = (RPredicate**) realloc (newObj->predicate, newObj->arraySize * sizeof(RPredicate*));
			}

			if(workload->isEqualityRanking){
				quick_sort((ElementType)newObj->predicate, newObj->arraySize, compare_eqrank_rpredicate);
				countEqualityTypePredicate(workload, newObj);

				if(workload->isInforceOeq && newObj->numEqualityLeft == 0)
					fatal_error("experiment generation is messed up!");

			}

			if(newObj->arraySize > maxObjDim)
				maxObjDim = newObj->arraySize;

			if(newObj->arraySize < minObjDim)
				minObjDim = newObj->arraySize;


			avgObjDim += newObj->arraySize;
			eqOpCount = countEQObject(newObj);
			workloadStat->avgEqOpCount += eqOpCount;
			workloadStat->avgNonEqOpCount += newObj->arraySize - eqOpCount;

			if(!test_generate_matching_probability(newObj, currObj)){
				print_single_object(newObj);
				print_single_object(currObj);
				fatal_error("test_generate_matching_probability failed!");
			}
			insert_first_LIST(newObj, objList);
			if(isEQObject(newObj)) workloadStat->eqObjectCount++;

			if(experimentParameter->dataDistributionType == DERIVED_OBJECTS_SELECTED_BASED_ON_ORIGINAL_DISTRIBUTIONS){
				free(predListScores);
				predListScores = NULL;
			}
		}
	}


	if(!workload->objectIsIncludeBaseTupe){
		objListPos = header_LIST( objList );
		prevObjListPos = objListPos;

		while((objListPos = advance_LIST(objListPos)) != NULL){
			currObj = retrieve_LIST(objListPos);

			if(currObj->id <= minID){
				remove_pos_LIST(objList, prevObjListPos, dummy_free);
				objListPos = prevObjListPos;
			}
			prevObjListPos = objListPos;
		}
	}


	workload->objectMinDim = minObjDim;
	workload->objectMaxDim = maxObjDim;
	workload->objectCount = get_size_LIST(objList);
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

}



void generate_matching_probability_equality_only(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, int duplicationDegree, int minID){
	ListPosition objListPos;
	Object *currObj, *newObj;
	float avgObjDim;
	int eqOpCount;
	int currDuplicationDegree;
	double keepingPredProbability;
	int i, newObjIndex;

	avgObjDim = 0;
	eqOpCount = 0;

	objListPos = header_LIST( objList );

	while((objListPos = advance_LIST(objListPos)) != NULL){
		currObj = retrieve_LIST(objListPos);

		eqOpCount = currObj->arraySize;
		workloadStat->avgEqOpCount += eqOpCount;
		workloadStat->avgNonEqOpCount += currObj->arraySize - eqOpCount;
		avgObjDim += currObj->arraySize;

		currDuplicationDegree = duplicationDegree;
		while(currDuplicationDegree--){

			newObj = clone_object(currObj);
			newObj->id = ++minID;
			newObj->arraySize = 0;
			newObj->numEqualityLeft = 0;

			keepingPredProbability = (double) equilikely_inclusive(workload->objectMinDim, workload->objectMaxDim) / (double)currObj->arraySize;

			newObjIndex = 0;
			for(i = 0; i < currObj->arraySize; i++){
				if(keepingPredProbability > uniform(0, 1)){
					newObj->predicate[newObjIndex] = newObj->predicate[i];
					if(newObjIndex != i) newObj->predicate[i] = NULL;
					newObj->arraySize++;
					newObj->numEqualityLeft++;
					newObjIndex++;
				}
			}

			if(newObj->arraySize < currObj->arraySize){
				newObj->active = (uint8_t*) realloc (newObj->active, newObj->arraySize * sizeof(uint8_t));
				newObj->covered = (uint8_t*) realloc (newObj->covered, newObj->arraySize * sizeof(uint8_t));
				newObj->score = (PredScore*) realloc (newObj->score, newObj->arraySize * sizeof(PredScore));
				newObj->predicate = (RPredicate**) realloc (newObj->predicate, newObj->arraySize * sizeof(RPredicate*));
			}

			insert_first_LIST(newObj, objList);

			avgObjDim += newObj->arraySize;
			eqOpCount = newObj->arraySize;
			workloadStat->avgEqOpCount += eqOpCount;
			workloadStat->avgNonEqOpCount += newObj->arraySize - eqOpCount;

		}
	}

	workload->objectCount = get_size_LIST(objList);
	workloadStat->eqObjectCount = workload->objectCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

}


List get_random_subset_minsize(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, uint32_t minSize){
	ListPosition currListPos, prevListPos;
	Object *currObj;
	float avgObjDim;
	int objCount, maxObjDim, minObjDim, eqObjCount, eqOpCount;


	avgObjDim = 0;
	objCount = 0;
	eqObjCount = 0;
	eqOpCount = 0;
	maxObjDim = 0;
	minObjDim = minSize;

	List resultSet;

	resultSet = make_empty_LIST( NULL, free );

	currListPos = header_LIST( objList );
	prevListPos = currListPos;

	while((currListPos = advance_LIST(currListPos)) != NULL){
		currObj = retrieve_LIST(currListPos);

		if(currObj->arraySize >= minSize){
			insert_last_LIST(currObj, resultSet);
			objCount++;
			avgObjDim += currObj->arraySize;

			if(currObj->arraySize > maxObjDim)
				maxObjDim = currObj->arraySize;

			if(currObj->arraySize < minObjDim)
				minObjDim = currObj->arraySize;

			if(isEQObject(currObj)) eqObjCount++;

			eqOpCount = countEQObject(currObj);
			workloadStat->avgEqOpCount += eqOpCount;
			workloadStat->avgNonEqOpCount += currObj->arraySize - eqOpCount;
			remove_pos_LIST(objList, prevListPos, dummy_free);
		}else{
			remove_pos_LIST(objList, prevListPos, delete_ptr_object_nopred);
		}
		currListPos = prevListPos;
	}

	workload->objectCount = objCount;
	workload->objectMaxDim = maxObjDim;
	workload->objectMinDim = minObjDim;
	workloadStat->eqObjectCount = eqObjCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

	return resultSet;
}



List get_random_subset_minsize_maxsize(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, int minSize, int maxSize){
	ListPosition currListPos, prevListPos;
	Object *currObj;
	float avgObjDim;
	int objCount, maxObjDim, minObjDim, eqObjCount, eqOpCount;


	avgObjDim = 0;
	objCount = 0;
	eqObjCount = 0;
	eqOpCount = 0;
	maxObjDim = 0;
	minObjDim = maxSize;

	List resultSet;

	resultSet = make_empty_LIST( NULL, free );

	currListPos = header_LIST( objList );
	prevListPos = currListPos;

	while((currListPos = advance_LIST(currListPos)) != NULL){
		currObj = retrieve_LIST(currListPos);

		if(currObj->arraySize >= minSize && currObj->arraySize <= maxSize){
			insert_last_LIST(currObj, resultSet);
			objCount++;
			avgObjDim += currObj->arraySize;

			if(currObj->arraySize > maxObjDim)
				maxObjDim = currObj->arraySize;


			if(currObj->arraySize < minObjDim)
				minObjDim = currObj->arraySize;


			if(isEQObject(currObj)) eqObjCount++;

			eqOpCount = countEQObject(currObj);
			workloadStat->avgEqOpCount += eqOpCount;
			workloadStat->avgNonEqOpCount += currObj->arraySize - eqOpCount;
			remove_pos_LIST(objList, prevListPos, dummy_free);
		}else{
			remove_pos_LIST(objList, prevListPos, delete_ptr_object_nopred);
		}
		currListPos = prevListPos;
	}

	workload->objectCount = objCount;
	workload->objectMaxDim = maxObjDim;
	workload->objectMinDim = minObjDim;
	workloadStat->eqObjectCount = eqObjCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

	return resultSet;
}


List get_random_subset_by_clonning(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, double selectionProbability, int isEqualityOnly){
	ListPosition P = header_LIST( objList );
	Object *currObj, *newObj;
	float avgObjDim;
	int objCount, maxObjDim, minObjDim, eqObjCount, eqOpCount;

	objCount = 0;
	avgObjDim = 0;
	eqObjCount = 0;
	eqOpCount = 0;
	maxObjDim = 0;
	minObjDim = MAX_INT;
	workloadStat->avgEqOpCount = 0;
	workloadStat->avgNonEqOpCount = 0;

	List resultSet;

	resultSet = make_empty_LIST( NULL, free );

	while((P = advance_LIST(P)) != NULL){
		currObj = retrieve_LIST(P);

		if(!isEqualityOnly || isEQObject(currObj)){
			if(selectionProbability >= uniform(0, 1)){
				newObj = clone_object(currObj);
				objCount++;

				if(newObj->arraySize > maxObjDim)
					maxObjDim = newObj->arraySize;

				if(newObj->arraySize < minObjDim)
					minObjDim = newObj->arraySize;

				if(isEQObject(newObj)) eqObjCount++;

				avgObjDim += newObj->arraySize;
				eqOpCount = countEQObject(newObj);
				workloadStat->avgEqOpCount += eqOpCount;
				workloadStat->avgNonEqOpCount += newObj->arraySize - eqOpCount;

				insert_last_LIST(newObj, resultSet);
			}
		}
	}


	workload->objectCount = objCount;
	workload->objectMaxDim = maxObjDim;
	workload->objectMinDim = minObjDim;
	workloadStat->eqObjectCount = eqObjCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

	return resultSet;
}


List get_random_subset(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, double selectionProbability, int isEqualityOnly){
	ListPosition P = header_LIST( objList );
	Object *currObj;
	float avgObjDim;
	int objCount, maxObjDim, minObjDim, eqObjCount, eqOpCount;

	avgObjDim = 0;
	objCount = 0;
	eqObjCount = 0;
	eqOpCount = 0;
	maxObjDim = 0;
	minObjDim = MAX_INT;

	List resultSet;

	resultSet = make_empty_LIST( NULL, free );

	while((P = advance_LIST(P)) != NULL){
		currObj = retrieve_LIST(P);

		if(!isEqualityOnly || isEQObject(currObj)){
			if(selectionProbability >= uniform(0, 1)){
				insert_last_LIST(currObj, resultSet);
				objCount++;
				avgObjDim += currObj->arraySize;

				if(currObj->arraySize > maxObjDim)
					maxObjDim = currObj->arraySize;

				if(currObj->arraySize < minObjDim)
					minObjDim = currObj->arraySize;

				if(isEQObject(currObj)) eqObjCount++;

				eqOpCount = countEQObject(currObj);
				workloadStat->avgEqOpCount += eqOpCount;
				workloadStat->avgNonEqOpCount += currObj->arraySize - eqOpCount;
			}
		}
	}

	workload->objectCount = objCount;
	workload->objectMaxDim = maxObjDim;
	workload->objectMinDim = minObjDim;
	workloadStat->eqObjectCount = eqObjCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

	return resultSet;
}


/* For equality objects only */
void drop_random_dim(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat){
	ListPosition P = header_LIST( objList );
	Object *currObj;
	float avgObjDim;
	int maxObjDim, minObjDim, eqObjCount, eqOpCount;
	double reductionProbability;
	int i, newObjIndex;

	avgObjDim = 0;
	eqObjCount = 0;
	eqOpCount = 0;
	maxObjDim = 0;
	minObjDim = MAX_INT;
	workloadStat->avgEqOpCount = 0;
	workloadStat->avgNonEqOpCount = 0;


	while((P = advance_LIST(P)) != NULL){
		currObj = retrieve_LIST(P);
		if(currObj->arraySize > workload->objectMaxDim){
			reductionProbability =  (workload->objectMinDim + workload->objectMaxDim) / 2 / (float)currObj->arraySize;

			newObjIndex = 0;
			currObj->numEqualityLeft = 0;

			for(i = 0; i < currObj->arraySize; i++){
				if((reductionProbability > uniform(0, 1) || (currObj->arraySize - i <= workload->objectMinDim - newObjIndex)) && newObjIndex < workload->objectMaxDim){
					currObj->predicate[newObjIndex] = currObj->predicate[i];
					currObj->active[newObjIndex] = currObj->active[i];
					currObj->covered[newObjIndex] = currObj->covered[i];
					currObj->score[newObjIndex] = currObj->score[i];
					if(newObjIndex != i) currObj->predicate[i] = NULL;
					if(currObj->predicate[newObjIndex]->operator == OPT_EQ)
						currObj->numEqualityLeft++;
					else
						fatal_error("drop_random_dim: Object cannot have non-equality predicate!");
					newObjIndex++;
				}
			}

			if(newObjIndex < currObj->arraySize){
				currObj->arraySize = newObjIndex;
				currObj->active = (uint8_t*) realloc (currObj->active, currObj->arraySize * sizeof(uint8_t));
				currObj->covered = (uint8_t*) realloc (currObj->covered, currObj->arraySize * sizeof(uint8_t));
				currObj->score = (PredScore*) realloc (currObj->score, currObj->arraySize * sizeof(PredScore));
				currObj->predicate = (RPredicate**) realloc (currObj->predicate, currObj->arraySize * sizeof(RPredicate*));
			}

		}

		if(currObj->arraySize > maxObjDim)
			maxObjDim = currObj->arraySize;

		if(currObj->arraySize < minObjDim)
			minObjDim = currObj->arraySize;

		if(isEQObject(currObj)) eqObjCount++;

		avgObjDim += currObj->arraySize;
		eqOpCount = countEQObject(currObj);
		workloadStat->avgEqOpCount += eqOpCount;
		workloadStat->avgNonEqOpCount += currObj->arraySize - eqOpCount;
	}

	workload->objectCount = get_size_LIST(objList);
	workload->objectMaxDim = maxObjDim;
	workload->objectMinDim = minObjDim;
	workloadStat->eqObjectCount = eqObjCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;
}




List reduce_dim(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat){
	ListPosition P = header_LIST( objList );
	Object *currObj, *newObj;
	float avgObjDim;
	int maxObjDim, eqObjCount, eqOpCount;
	int newDim;
	double reductionProbability;
	int i, newObjIndex;

	avgObjDim = 0;
	eqObjCount = 0;
	eqOpCount = 0;
	maxObjDim = 0;

	List resultSet;

	resultSet = make_empty_LIST( NULL, free );

	while((P = advance_LIST(P)) != NULL){
		currObj = retrieve_LIST(P);
		newDim = equilikely_inclusive(workload->objectMinDim, workload->objectMaxDim);
		newObj = clone_object(currObj);
		reductionProbability = newDim / (float)currObj->arraySize;


		/* Make sure the new operator has at least minDim */
		if(newDim < currObj->arraySize){
			do{
				newObjIndex = 0;
				newObj->arraySize = 0;
				newObj->numEqualityLeft = 0;

				for(i = 0; i < currObj->arraySize; i++){
					if(reductionProbability > uniform(0, 1) && newObjIndex < workload->objectMaxDim){
						newObj->predicate[newObjIndex] = currObj->predicate[i];
						newObj->active[newObjIndex] = currObj->active[i];
						newObj->covered[newObjIndex] = currObj->covered[i];
						newObj->score[newObjIndex] = currObj->score[i];
						if(newObjIndex != i) newObj->predicate[i] = NULL;
						newObj->arraySize++;
						if(newObj->predicate[newObjIndex]->operator == OPT_EQ) newObj->numEqualityLeft++;
						newObjIndex++;
					}
				}
			}while(newObjIndex < workload->objectMinDim);
		}

		if(newObj->arraySize < currObj->arraySize){
			newObj->active = (uint8_t*) realloc (newObj->active, newObj->arraySize * sizeof(uint8_t));
			newObj->covered = (uint8_t*) realloc (newObj->covered, newObj->arraySize * sizeof(uint8_t));
			newObj->score = (PredScore*) realloc (newObj->score, newObj->arraySize * sizeof(PredScore));
			newObj->predicate = (RPredicate**) realloc (newObj->predicate, newObj->arraySize * sizeof(RPredicate*));
		}

		insert_last_LIST(newObj, resultSet);

		if(newObj->arraySize > maxObjDim)
			maxObjDim = newObj->arraySize;

		if(isEQObject(newObj)) eqObjCount++;

		avgObjDim += newObj->arraySize;
		eqOpCount = countEQObject(newObj);
		workloadStat->avgEqOpCount += eqOpCount;
		workloadStat->avgNonEqOpCount += newObj->arraySize - eqOpCount;
	}

	workload->objectCount = get_size_LIST(resultSet);
	workload->objectMaxDim = maxObjDim;
	workload->objectMinDim = workload->objectMinDim;
	workloadStat->eqObjectCount = eqObjCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

	return resultSet;
}



static void scramble_preds(Object *obj){
	struct hashtable_itr *iterator;
	struct hashtable *scrambleIndex;
	uint32_t *currID;
	RPredicateScorePair *currPredIndex;
	int currIndex;
	uint32_t tmp;


	scrambleIndex = create_hashtable(obj->arraySize * 5, hash_robert_jenkins_rpredicate, compare_rpredicate_keys);

	for(currIndex = 0; currIndex < obj->arraySize; currIndex++){
		do{
		 	tmp = equilikely(1, RAND_MAX);
		}while(NULL != search_scramble_pred_list(scrambleIndex, &tmp));

		currID = malloc(sizeof(uint32_t));
		*currID = tmp;

		currPredIndex = malloc(sizeof(RPredicateScorePair));
		currPredIndex->predicate = obj->predicate[currIndex];
		currPredIndex->score = obj->score[currIndex];



		if (!insert_scramble_pred_list(scrambleIndex, currID, currPredIndex)) fatal_error("Cannot insert into scramble Index!");
	}

	currIndex = 0;
	iterator = hashtable_iterator(scrambleIndex);
	if (hashtable_count(scrambleIndex) > 0){
		do {
			currPredIndex = hashtable_iterator_value(iterator);
			obj->predicate[currIndex] = currPredIndex->predicate;
			obj->score[currIndex] = currPredIndex->score;
			currIndex++;
		} while (hashtable_iterator_advance(iterator));
	}

	free(iterator);
	iterator = NULL;

	hashtable_destroy(scrambleIndex, FALSE, free);
}



void scramble_obj_pred_list(List objList){
	ListPosition currListPos;
	Object *currObj;

	currListPos = header_LIST( objList );
	while((currListPos = advance_LIST(currListPos)) != NULL){
		currObj = retrieve_LIST(currListPos);
		scramble_preds(currObj);
	}
}



List scramble_objects_list(List objList){
	ListPosition currListPos;
	Object *currObj;
	struct hashtable_itr *iterator;
	struct hashtable *scrambleIndex;
	uint32_t *currID;

	List resultSet;

	scrambleIndex = create_hashtable(get_size_LIST(objList) * 1.5, hash_thomas_wang_num, equal_obj_keys);
	resultSet = make_empty_LIST( NULL, free );

	currListPos = header_LIST( objList );
	while((currListPos = advance_LIST(currListPos)) != NULL){
		currObj = retrieve_LIST(currListPos);

		if (NULL == search_scramble_obj_list(scrambleIndex, &currObj->id)){
			currID = malloc(sizeof(uint32_t));
			*currID = currObj->id;

			if (!insert_scramble_obj_list(scrambleIndex, currID, currObj)) fatal_error("Cannot insert into scramble Index!");
		}else{
			fatal_error("Sub ID must be unique!");
		}

	}


	iterator = hashtable_iterator(scrambleIndex);

	if (hashtable_count(scrambleIndex) > 0){
		do {
			currObj = hashtable_iterator_value(iterator);
			insert_last_LIST(currObj, resultSet);
		} while (hashtable_iterator_advance(iterator));
	}

	free(iterator);
	iterator = NULL;

	delete_list_pointer_LIST(objList);
	hashtable_destroy(scrambleIndex, FALSE, free);

	return resultSet;
}


void index_objects(List objList){
	Object *currObj;
	ListPosition currListPos;


	currListPos = header_LIST( objList );
	while((currListPos = advance_LIST(currListPos)) != NULL){
		currObj = retrieve_LIST(currListPos);
		index_object(currObj);
	}
}


void remove_random_subset(List objList, WorkloadParameter* workload, double minNumObject){
	ListPosition currListPos, prevListPos;
	Object *currObj;
	double keepingProbability;
	int selectedObjCount = 0;

	keepingProbability = (float) minNumObject / (float)get_size_LIST(objList);
	currListPos = header_LIST( objList );
	prevListPos = currListPos;

	while((currListPos = advance_LIST(currListPos)) != NULL){
		selectedObjCount++;
		currObj = retrieve_LIST(currListPos);
		if((keepingProbability <= uniform(0, 1) && get_size_LIST(objList) >= minNumObject) || (selectedObjCount >= minNumObject)){
			remove_pos_LIST(objList, prevListPos, delete_ptr_object_nopred);
			currListPos = prevListPos;
			selectedObjCount--;
		}
		prevListPos = currListPos;
	}

	workload->objectCount = get_size_LIST(objList);
}


static void print_generic_list_object(List subList, Object *currEvent, int count, int isSingleLine){
	ListPosition P = header_LIST( subList );
	Object *currSub;

	if( is_empty_LIST( subList ) )
		printf( "Empty list\n" );
	else
	{
		while((P = advance_LIST(P)) != NULL && count != 0){
			currSub = retrieve_LIST(P);
			if(isSingleLine){
				if(currEvent != NULL)
					print_single_object_with_score(currSub, currEvent);
				else
					print_single_object(currSub);
			}else{
				if(currEvent != NULL)
					print_object_with_score(currSub, currEvent);
				else
					print_object(currSub);
			}
			count--;
		}
	}
}



void print_object_id(List subList, int objID){
	ListPosition P = header_LIST( subList );
	Object *currSub;

	if( is_empty_LIST( subList ) )
		printf( "Empty list\n" );
	else
	{
		while((P = advance_LIST(P)) != NULL){
			currSub = retrieve_LIST(P);

			if(currSub->id == objID){
				print_single_object(currSub);
			}
		}
	}
}


void print_list_single_object(List subList, int count){
	print_generic_list_object(subList, NULL, count, TRUE);
}


void print_list_object(List subList, int count){
	print_generic_list_object(subList, NULL, count, FALSE);
}


void print_list_object_with_score(List subList, Object *currEvent, int count){
	print_generic_list_object(subList, currEvent, count, FALSE);
}


void write_list_object_to_file(const WorkloadParameter* workload, WorkloadStat* workloadStat, List objectList) {
	FILE *fp;
	ListPosition objectPos;
	Object *currObject;
	RPredicate *currPred;
	PredScore *value, defaultValue;
	AttrValuePair currAttrValuePair;
	int dimCounter, predCounter, valueCounter;

	fp = fopen(workload->objectOutput, "w");
	if (fp == NULL) {
		printf("ERROR in creating output file to write objects (%s) \n", workload->objectOutput);
		exit(1);
	}

	objectPos = header_LIST( objectList );


	fprintf(fp, "$systemDim %d$\n", workload->systemDim);
	fprintf(fp, "$systemDistinctValue %d$\n", workload->systemDistinctValue);

	for(dimCounter = 0; dimCounter < workload->systemDim + 1; dimCounter++){
		currAttrValuePair.attribute = dimCounter;
		currAttrValuePair.value = 0;

		defaultValue = 0;
		if((value = search_frequency(workload->idfHashIndex, &currAttrValuePair)) != NULL)
			defaultValue = *value;

		fprintf(fp, "%d %f; ", workload->distinctValueIndex[dimCounter], defaultValue);
	}

	fprintf(fp, "\n$ObjectCount %d$\n", get_size_LIST(objectList));
	fprintf(fp, "$ObjectMaxDim %d$\n", workload->objectMaxDim);
	fprintf(fp, "$ObjectMinDim %d$\n", workload->objectMinDim);
	fprintf(fp, "$EqObjectCount$ %d$\n", workloadStat->eqObjectCount);
	fprintf(fp, "$AvgEqOpCount %.2f$\n", workloadStat->avgEqOpCount);
	fprintf(fp, "$AvgNonEqOpCount %.2f$\n", workloadStat->avgNonEqOpCount);
	fprintf(fp, "$AvgObjectDim %.2f$\n", workloadStat->avgObjectDim);

	/* Iterate through each object and write them to the file */
	while((objectPos = advance_LIST(objectPos)) != NULL){
		currObject = retrieve_LIST(objectPos);

		/* Write object header */
		fprintf(fp, "[%u, %u, %u]\t", currObject->id, currObject->arraySize, currObject->numEqualityLeft);


		for(predCounter = 0; predCounter < currObject->arraySize; predCounter++){
			fprintf(fp, "%u %u %f |", currObject->active[predCounter], currObject->covered[predCounter], currObject->score[predCounter]);
		}

		for(predCounter = 0; predCounter < currObject->arraySize; predCounter++){
			currPred = currObject->predicate[predCounter];

			assert(currPred->valuesBoundary.minBoundary >= 0);
			assert(currPred->valuesBoundary.maxBoundary <= workload->distinctValueIndex[currPred->attribute]);

			fprintf(fp, "%u, %u, %u, %u, %u %u, ", currPred->id, currPred->valuesArraySize, currPred->attribute, currPred->operator, currPred->valuesBoundary.minBoundary, currPred->valuesBoundary.maxBoundary);

			for(valueCounter = 0; valueCounter < currPred->valuesArraySize; valueCounter++){
				fprintf(fp, "%u ", currPred->values[valueCounter]);
			}
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}


Object* convert_object_stream_to_object(const WorkloadParameter* workload, ObjectStream *objectStream, struct hashtable *predicateIndex){
	Object *object;
	RPredicate *currPred;
	RPredicate *oldPred;
	RPredicateStream *currPredStream;

	int predCounter, valueCounter;

	object = malloc(sizeof(Object));
	object->id = objectStream->id;
	object->predicate = malloc(sizeof(RPredicate*) * objectStream->arraySize);
	object->active = malloc(sizeof(uint8_t) * objectStream->arraySize);
	object->covered = malloc(sizeof(uint8_t) * objectStream->arraySize);
	object->score = malloc(sizeof(PredScore) * objectStream->arraySize);
	object->arraySize = objectStream->arraySize;
	object->numEqualityLeft = objectStream->numEqualityLeft;

	for(predCounter = 0; predCounter < objectStream->arraySize; predCounter++){
		currPredStream = objectStream->predicate[predCounter];

		object->active[predCounter] = objectStream->active[predCounter];
		object->covered[predCounter] = objectStream->covered[predCounter];
		object->score[predCounter] = objectStream->score[predCounter];

		currPred = malloc(sizeof(RPredicate));
		currPred->values = malloc(sizeof(PredValue) * currPredStream->valuesArraySize );
		currPred->id = currPredStream->id;
		currPred->attribute = currPredStream->attribute;
		currPred->operator = currPredStream->operator;
		currPred->valuesBoundary.minBoundary = currPredStream->valuesBoundary.minBoundary;
		currPred->valuesBoundary.maxBoundary = currPredStream->valuesBoundary.maxBoundary;
		currPred->valuesArraySize = currPredStream->valuesArraySize;


		assert(currPred->valuesBoundary.minBoundary >= 0);
		assert(currPred->valuesBoundary.maxBoundary <= workload->distinctValueIndex[currPred->attribute]);


		for(valueCounter = 0; valueCounter < currPredStream->valuesArraySize; valueCounter++){
			currPred->values[valueCounter] = currPredStream->values[valueCounter];
		}

		if ((oldPred = search_predicate(predicateIndex, currPred)) == NULL){
			if (!insert_predicate(predicateIndex, currPred, currPred)) fatal_error("Cannot insert into predicate into  Predicate Index (hashtable)!");
			object->predicate[predCounter] = currPred;
		}else{
			free(currPred->values);
			free(currPred);
			object->predicate[predCounter] = oldPred;
		}
	}

	return object;
}


List read_object_to_list(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex, int isEnabledDistinctIndexIdfIndexCreation) {
	FILE *fp;
	List objectList;
	int dimCounter, objectCount, returnFp;

	Object *currObject;
	ObjectStream *currObjectStream;
	RPredicateStream *currPredStream;

	float score;

	int predCounter, valueCounter;

	fp = fopen(workload->objectOutput, "r");
	if (fp == NULL) {
		printf("ERROR in opening the output file to read objects (%s) \n", workload->objectOutput);
		exit(1);
	}
	objectList = make_empty_pointer_LIST(NULL);


	returnFp = fscanf(fp, "$systemDim %d$\n", &workload->systemDim);
	returnFp = fscanf(fp, "$systemDistinctValue %d$\n", &workload->systemDistinctValue);

	if(isEnabledDistinctIndexIdfIndexCreation){
		workload->distinctValueIndex = malloc(sizeof(int) * workload->systemDim + 1);
		workload->idfIndex = malloc(sizeof(PredScore) * workload->systemDim + 1);
		ini_predscore_array(workload->idfIndex, workload->systemDim + 1);
	}

	for(dimCounter = 0; dimCounter < workload->systemDim + 1; dimCounter++){
		returnFp = fscanf(fp, "%d %f; ", &workload->distinctValueIndex[dimCounter], &score);
		workload->idfIndex[dimCounter] = (PredScore) score;
	}

	returnFp = fscanf(fp, "\n$ObjectCount %d$\n", &workload->objectCount);
	returnFp = fscanf(fp, "$ObjectMaxDim %d$\n", &workload->objectMaxDim);
	returnFp = fscanf(fp, "$ObjectMinDim %d$\n", &workload->objectMinDim);
	returnFp = fscanf(fp, "$EqObjectCount$ %d$\n", &workloadStat->eqObjectCount);
	returnFp = fscanf(fp, "$AvgEqOpCount %f$\n", &workloadStat->avgEqOpCount);
	returnFp = fscanf(fp, "$AvgNonEqOpCount %f$\n", &workloadStat->avgNonEqOpCount);
	returnFp = fscanf(fp, "$AvgObjectDim %f$\n", &workloadStat->avgObjectDim);

	objectCount = workload->objectCount;
	/* Iterate through each object and write them to the file */
	while(objectCount-- > 0){
		currObjectStream = malloc(sizeof(ObjectStream));

		/* Write object header */
		returnFp = fscanf(fp, "[%u, %u, %u]\t", &currObjectStream->id, &currObjectStream->arraySize, &currObjectStream->numEqualityLeft);
		currObjectStream->predicate = malloc(sizeof(RPredicateStream*) * currObjectStream->arraySize);
		currObjectStream->active = malloc(sizeof(uint32_t) * currObjectStream->arraySize);
		currObjectStream->covered = malloc(sizeof(uint32_t) * currObjectStream->arraySize);
		currObjectStream->score = malloc(sizeof(PredScore) * currObjectStream->arraySize);

		for(predCounter = 0; predCounter < currObjectStream->arraySize; predCounter++){
			returnFp = fscanf(fp, "%u %u %f |", &currObjectStream->active[predCounter], &currObjectStream->covered[predCounter], &score);
			currObjectStream->score[predCounter] = (PredScore) score;
		}


		for(predCounter = 0; predCounter < currObjectStream->arraySize; predCounter++){
			currPredStream = malloc(sizeof(RPredicateStream));

			returnFp = fscanf(fp, "%u, %u, %u, %u, %u %u, ", &currPredStream->id, &currPredStream->valuesArraySize, &currPredStream->attribute, &currPredStream->operator, &currPredStream->valuesBoundary.minBoundary, &currPredStream->valuesBoundary.maxBoundary);
			currPredStream->values = malloc(sizeof(uint32_t) * currPredStream->valuesArraySize);

			assert(currPredStream->valuesBoundary.minBoundary >= 0);
			assert(currPredStream->valuesBoundary.maxBoundary <= workload->distinctValueIndex[currPredStream->attribute]);

			for(valueCounter = 0; valueCounter < currPredStream->valuesArraySize; valueCounter++){
				returnFp = fscanf(fp, "%u ", &currPredStream->values[valueCounter]);
			}

			currObjectStream->predicate[predCounter] = currPredStream;
		}

		currObject = convert_object_stream_to_object(workload, currObjectStream, predicateIndex);

		delete_ptr_objectStream(currObjectStream);
		currObjectStream = NULL;
		insert_last_LIST(currObject, objectList);
	}

	fclose(fp);

	return objectList;
}


int sanity_test_obj(Object *obj){
	int i, j;
	RPredicate *currPred, *nextPred;

	for(i = 0; i < obj->arraySize; i++){
		currPred = obj->predicate[i];
		if(currPred == NULL){
			error("Obj compression failed!");
			return FALSE;
		}

		for(j = 0; j < currPred->valuesArraySize; j++){
			if(currPred->values[j] == 0){
				error("Obj value must be greater than 0!");
				return FALSE;
			}
			if(j > 0 && currPred->values[j-1] > currPred->values[j]){
				error("Obj value sorting failed!");
				return FALSE;
			}
		}

		if(currPred->valuesBoundary.minBoundary != currPred->values[0] && currPred->valuesBoundary.maxBoundary != currPred->values[currPred->valuesArraySize - 1]){
			error("Obj boundary assignment failed!");
			return FALSE;
		}

		if(currPred->attribute == 0){
			error("Obj attribute must be greater than 0!");
			return FALSE;
		}

		for(j = i + 1; j < obj->arraySize; j++){
			nextPred = obj->predicate[j];
			if(currPred->attribute == nextPred->attribute){
				error("Obj merging failed!");
				return FALSE;
			}
		}

		if(currPred->operator == OPT_EQ && currPred->valuesArraySize > 1){
			error("Obj EQ operator assignment failed!");
			return FALSE;
		}

		if(currPred->operator == OPT_IN && currPred->valuesArraySize == 1){
			error("Obj IN operator assignment failed!");
			return FALSE;
		}


		if(obj->active[i] != TRUE || obj->covered[i] != FALSE){
			error("Obj active and covered assingment failed!");
			return FALSE;
		}
	}
	return TRUE;
}


/* removes extra space, newline, and non-alphanumeric char and converts to lower*/
int filter_readline(char *src, char *dest, int size){
	int i, j;
	i = j = 0;

	strcpy(dest, "");

	for ( i = 0; i < size; i++ ){
		if ( src[i] == '\n' ){
			dest[j++] = '\0';
			return j;
		}else if (isalpha(src[i])){
			dest[j++] = tolower(src[i]);
		}else if(isspace(src[i])){
			if(j == 0 || !isspace(dest[j-1]))
				dest[j++] = src[i];
		}
	}

	return j;
}

int merge_equality_object(Object *obj, int currDimIndex, struct hashtable *predicateIndex){
	int i, j, currValue, lastValue, size, countEquality;
	int isNewValue, isNewValueAdded;
	RPredicate *currPred, *nextPred, *oldPred, *newPred;

	for(i = 0; i < currDimIndex; i++){
		if(obj->predicate[i] != NULL){
			currPred = obj->predicate[i];
			isNewValue = TRUE;
			for(j = i + 1; j < currDimIndex; j++){
				nextPred = obj->predicate[j];

				/* find matching predicate with equal attribute value */
				if(nextPred != NULL && currPred->attribute == nextPred->attribute){
					if(FALSE){
						/* Search to see if the value of nextPred is already covered by currPred or not. */
						for(currValue = 0; currValue < currPred->valuesArraySize; currValue++){
							if(currPred->values[currValue] == nextPred->values[0]){
								isNewValue = FALSE;
								break;
							}
						}

						/* if new value is found, then re-create the value list, and sort them */
						if(isNewValue){
							newPred = malloc(sizeof(RPredicate));
							newPred->attribute = currPred->attribute;
							newPred->valuesArraySize = currPred->valuesArraySize + 1;
							newPred->values = malloc(sizeof(PredValue) * newPred->valuesArraySize );
							newPred->operator = OPT_IN;
							newPred->id = newPred->attribute * DIMENSION_SHIFTING_FACTOR + newPred->operator * OPERATORS_SHIFTING_FACTOR;

							lastValue = 0;
							isNewValueAdded = FALSE;
							for(currValue = 0; currValue < newPred->valuesArraySize; currValue++){
								if(lastValue < currPred->valuesArraySize && (currPred->values[lastValue] < nextPred->values[0] || isNewValueAdded)){
									newPred->values[currValue] = currPred->values[lastValue++];
								}else{
									newPred->values[currValue] = nextPred->values[0];
									isNewValueAdded = TRUE;
								}
								newPred->id += newPred->values[currValue]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
							}

							setBoundary(&newPred->valuesBoundary, newPred->values[0], newPred->values[newPred->valuesArraySize - 1]);


							if ((oldPred = search_predicate(predicateIndex, newPred)) == NULL){
								if (!insert_predicate(predicateIndex, newPred, newPred)) fatal_error("Cannot insert into predicate into  Predicate Index (hashtable)!");
								obj->predicate[i] = newPred;
							}else{
								free(newPred->values);
								free(newPred);
								obj->predicate[i] = oldPred;
							}
						}
					}
					obj->predicate[j] = NULL;
				}
			}
		}
	}


	/* Compressing the obj */
	size = 0;
	countEquality = 0;
	for(i = 0; i < currDimIndex; i++){
		if(obj->predicate[i] != NULL){
			if(obj->predicate[i]->operator == OPT_EQ) countEquality++;
			obj->predicate[size++] = obj->predicate[i];
		}
	}

	obj->numEqualityLeft = countEquality;

	return size;
}

static Object* construct_qgram(WorkloadParameter* workload, char *str, int size, int objID, struct hashtable *dimIndex, struct hashtable *predicateIndex){
	int i, j;
	i = j = 0;
	char qgram[4];
	char key[3];
	PredValue value;
	int *dim, currDimIndex;
	Object *obj;
	RPredicate *currPred, *oldPred;

	if(size < 3)
		return NULL;

	obj = malloc(sizeof(Object));
	obj->id = objID;
	obj->predicate = malloc(sizeof(RPredicate*) * size);
	obj->active = malloc(sizeof(uint8_t) * size);
	obj->covered = malloc(sizeof(uint8_t) * size);
	obj->score = malloc(sizeof(PredScore) * size);
	obj->arraySize = MAX_NUMBER_ARRAY_VALUE_SIZE;
	obj->numEqualityLeft = 0;


	currDimIndex = 0;
	for ( i = 0; i < size - 3; i++ ){
		if(!isspace(str[i]) && !isspace(str[i + 1]) && !isspace(str[i + 2])){
			currPred = malloc(sizeof(RPredicate));
			currPred->valuesArraySize = 1;
			currPred->values = malloc(sizeof(PredValue) * currPred->valuesArraySize );

			/* All predicates generated in this method are implicitly base predicate.
			 * To indicate that a predicate is a base predicate, we point it to itself.
			 */
			currPred->baseRPredicate = currPred;

			qgram[0] = str[i];
			qgram[1] = str[i + 1];
			qgram[2] = str[i + 2];
			qgram[3] = '\0';

			key[0] = str[i];
			key[1] = str[i + 1];
			key[2] = '\0';
			dim = search_dim(dimIndex, key);


			if(workload->qgramID == QGRAM_BASE_THREE_GRAM){
				value = str[i + 2] - 'a' + 1;
			}else if(workload->qgramID == QGRAM_THREE_GRAM_EXTENDED_DOMAIN_SIZE){
				value = (str[i] - 'a' + 1)*26*26 + (str[i + 1] - 'a' + 1)*26 + (str[i + 2] - 'a' + 1);
			}else{
				fatal_error("Invalid qgramID generation value!");
			}

			currPred->operator = OPT_EQ;
			currPred->attribute = *dim;
			currPred->values[0] = value;
			setBoundary(&currPred->valuesBoundary, currPred->values[0], currPred->values[0]);
			currPred->id = currPred->attribute * DIMENSION_SHIFTING_FACTOR + currPred->operator * OPERATORS_SHIFTING_FACTOR + currPred->values[0] / DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;

			if ((oldPred = search_predicate(predicateIndex, currPred)) == NULL){
				if (!insert_predicate(predicateIndex, currPred, currPred)) fatal_error("Cannot insert into predicate into  Predicate Index (hashtable)!");
				obj->predicate[currDimIndex] = currPred;
			}else{
				free(currPred->values);
				free(currPred);
				obj->predicate[currDimIndex] = oldPred;
			}

			currDimIndex++;

			/* Max size string is 250 */
			if(MAX_NUMBER_ARRAY_VALUE_SIZE == currDimIndex) break;
		}
	}

	if(currDimIndex == 0){
		return NULL;
	}


	currDimIndex = merge_equality_object(obj, currDimIndex, predicateIndex);

	if(currDimIndex < obj->arraySize){
		obj->active = (uint8_t*) realloc (obj->active, currDimIndex * sizeof(uint8_t));
		obj->covered = (uint8_t*) realloc (obj->covered, currDimIndex * sizeof(uint8_t));
		obj->score = (PredScore*) realloc (obj->score, currDimIndex * sizeof(PredScore));
		obj->predicate = (RPredicate**) realloc (obj->predicate, currDimIndex * sizeof(RPredicate*));
		obj->arraySize = currDimIndex;
	}


	for(i = 0; i < obj->arraySize; i++){
		obj->active[i] = TRUE;
		obj->covered[i] = FALSE;
	}

	if(countEQObject(obj) == 0){
		error("Found string/object with zero equality!");
		return NULL;
	}

	return obj;
}



List generate_qgram_workload_helper(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex, int minDimSize, int maxDimSize){
	FILE *fp;
	List objectList;
	int i, j;
	Object *currObject;
	int dimCounter, objCounter;
	char readLine[10000];
	char filteredLine[10000];
	char *key;
	int *value;
	int size;
	struct hashtable *duplicateFreeIndex, *dimIndex;
	struct hashtable_itr *iterator;
	double avgObjDim;
	int maxObjDim, minObjDim;
	int eqObjectCount;
	int eqOpCount;
	int countLines;
	int isCurrObjAccepted;


	dimIndex = create_hashtable(1000, hash_thomas_wang_str, is_equal_str_keys);
	duplicateFreeIndex = create_hashtable(1000000, hash_thomas_wang_str, is_equal_str_keys);

	fp = fopen(workload->objectInput, "r");
	if (fp == NULL) {
		printf("ERROR in reading input file (%s) \n", workload->objectInput);
		exit(1);
	}
	objectList = make_empty_pointer_LIST(NULL);

	/* Go through the file line by line*/
	countLines = 0;
	while(!feof(fp)){
		if(fgets (readLine , 10000, fp) != NULL){
			size = filter_readline(readLine, filteredLine, 10000);

			/* Remove duplicate */
			if (NULL == search_str(duplicateFreeIndex, filteredLine)){
				key = malloc(sizeof(char) * size);
				value = malloc(sizeof(int));

				*value = size;
				strcpy(key, filteredLine);
				if (!insert_str(duplicateFreeIndex, key, value)) fatal_error("Cannot insert into duplicate free hashtable!");
				countLines++;
			}
		}
	}
	fclose(fp);


	dimCounter = 1;
	/* Construct dimIndex */
	for(i = 0; i < 26; i++){
		for(j = 0; j < 26; j++){
			key = malloc(sizeof(char) * 3);
			value = malloc(sizeof(int));

			*value = dimCounter++;
			key[0] = 'a' + i;
			key[1] = 'a' + j;
			key[2] = '\0';

			if (!insert_dim(dimIndex, key, value)) fatal_error("Cannot insert into dim index hashtable!");
		}
	}

	objCounter = 1;
	avgObjDim = 0;
	maxObjDim = 0;
	minObjDim = MAX_NUMBER_ARRAY_VALUE_SIZE;
	eqObjectCount = 0;
	iterator = hashtable_iterator(duplicateFreeIndex);
	if (hashtable_count(duplicateFreeIndex) > 0){
		do {
			key = hashtable_iterator_key(iterator);
			value = hashtable_iterator_value(iterator);

			isCurrObjAccepted = FALSE;

			if((currObject = construct_qgram(workload, key, *value, objCounter, dimIndex, predicateIndex)) != NULL){
				if(minDimSize == 0 && maxDimSize == 0){
					isCurrObjAccepted = TRUE;
				}else if(minDimSize != 0 && maxDimSize == 0){
					if(currObject->arraySize >= minDimSize)
						isCurrObjAccepted = TRUE;
				}else if(minDimSize != 0 && maxDimSize != 0){
					if(currObject->arraySize >= minDimSize && currObject->arraySize <= maxDimSize)
						isCurrObjAccepted = TRUE;
				}else{
					fatal_error("Unknown error in qgram construction!");
				}

				if(isCurrObjAccepted){
					insert_last_LIST(currObject, objectList);
					avgObjDim += currObject->arraySize;

					if(currObject->arraySize > maxObjDim)
						maxObjDim = currObject->arraySize;

					if(currObject->arraySize < minObjDim)
						minObjDim = currObject->arraySize;

					if(currObject->numEqualityLeft == currObject->arraySize) eqObjectCount++;

					if(workload->isEqualityRanking){
						quick_sort((ElementType)currObject->predicate, currObject->arraySize, compare_eqrank_rpredicate);
						countEqualityTypePredicate(workload, currObject);

						if(workload->isInforceOeq && currObject->numEqualityLeft == 0)
							fatal_error("experiment generation is messed up!");
					}else{
						currObject->numEqualityLeft = 0;
					}

					if(!sanity_test_obj(currObject)) fatal_error("sanity test for qgram construction failed!");
					objCounter++;

					eqOpCount = countEQObject(currObject);
					workloadStat->avgEqOpCount += eqOpCount;
					workloadStat->avgNonEqOpCount += currObject->arraySize - eqOpCount;
				}else{
					delete_ptr_object_nopred(currObject);
				}
			}

		} while (hashtable_iterator_advance(iterator) && objCounter <= workload->objectCount);
	}


	workload->systemDim = dimCounter;

	if(workload->qgramID == QGRAM_BASE_THREE_GRAM){
		workload->systemDistinctValue = 26;
	}else if(workload->qgramID == QGRAM_THREE_GRAM_EXTENDED_DOMAIN_SIZE){
		workload->systemDistinctValue = 26*26*26 + 26*26 + 26;
	}else{
		fatal_error("Invalid qgramID generation value!");
	}

	workload->distinctValueIndex = malloc(sizeof(int) * workload->systemDim + 1);
	ini_predvalue_array_to_value(workload->distinctValueIndex, workload->systemDim + 1, workload->systemDistinctValue);
	workload->objectCount = objCounter - 1;
	workload->objectMaxDim = maxObjDim;
	workload->objectMinDim = minObjDim;
	workloadStat->eqObjectCount = eqObjectCount;
	workloadStat->avgEqOpCount = workloadStat->avgEqOpCount/(float)workload->objectCount;
	workloadStat->avgNonEqOpCount = workloadStat->avgNonEqOpCount/(float)workload->objectCount;
	workloadStat->avgObjectDim = avgObjDim / (float) workload->objectCount;

	hashtable_destroy(duplicateFreeIndex, TRUE, free);
	hashtable_destroy(dimIndex, TRUE, free);

	return objectList;
}


List generate_qgram_workload(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex){
	return generate_qgram_workload_helper(workload, workloadStat, predicateIndex, 0, 0);
}


List generate_qgram_workload_minsize(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex, int minDimSize){
	return generate_qgram_workload_helper(workload, workloadStat, predicateIndex, minDimSize, 0);
}


List generate_qgram_workload_minsize_maxsize(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex, int minDimSize, int maxDimSize){
	return generate_qgram_workload_helper(workload, workloadStat, predicateIndex, minDimSize, maxDimSize);
}


void print_list_event(List subList, int count){
	ListPosition P = header_LIST( subList );
	Event *currEvent;
	RPredicate *currPred;
	HashPosition eventPredPosition;
	HashIterator iterator;

	if( is_empty_LIST( subList ) )
		printf( "Empty list\n" );
	else
	{
		while((P = advance_LIST(P)) != NULL && count != 0){
			currEvent = retrieve_LIST(P);
			printf("Event id: %d --> ", currEvent->id);

			iterator = get_iterator_HASH(currEvent->predicateHash);

			/* Iterator through event predicate lists */
			while(iterator_has_next_HASH(iterator)){
				eventPredPosition = iterator_next_HASH(iterator);
				while((eventPredPosition = advance_HASH(eventPredPosition)) != NULL){
					currPred = (RPredicate*)retrieve_value_HASH( eventPredPosition );
					printf("Pred addr: %ld, id: %d, attr: %d, op: %d, v: %d, s: %d \t range [%d, %d] ", (long int)currPred, currPred->id, currPred->attribute, currPred->operator, currPred->values[0], currPred->valuesArraySize, currPred->valuesBoundary.minBoundary, currPred->valuesBoundary.maxBoundary);
				}
			}
			puts("");
			count--;
		}
	}
}


List random_event_rpredicate_uniform(const ExperimentParameter* experiment, struct hashtable *predicateIndex){
	Event* currEvent;
	RPredicate *currPred, *oldPred;
	int i, j;
	HashTable tmpHash;
	int* currValue;
	List eventList = make_empty_LIST( NULL, free );

	if(experiment->systemDim > MAX_NUMBER_OF_DIMENSION)
		fatal_error("The number of requested dimension exceeds the maximum number dimensions allowed by the system!");

	for(i = 0; i < experiment->eventCount; i++ ){
		currEvent = malloc(sizeof(Event));
		currEvent->id = i;
		currEvent->predicateHash = NULL;
		if(experiment->isEventHashEnabled) currEvent->predicateHash = create_HASH( experiment->systemDim );
		currEvent->predicateList = make_empty_LIST(NULL, dummy_free);
		tmpHash = create_HASH( experiment->eventMaxDim );

		for(j = 0; j < experiment->eventMaxDim; j++ ){
			currValue = malloc(sizeof(int));

			currPred = malloc(sizeof(RPredicate));
			currPred->operator = OPT_EQ;

			if(experiment->eventUniform)
				currPred->attribute = equilikely_inclusive(1, experiment->systemDim);
			else if(experiment->eventZip)
				currPred->attribute = zipf_first_instance(experiment->eventZipAlpha, experiment->systemDim);
			else
				fatal_error("Please specify the underlining distribution of the event experiment!");


			*currValue = currPred->attribute;

			if(find_HASH(currValue, tmpHash, hash_key_ptr_int, compare_ptr_int) == NULL){
				insert_HASH(currValue, currValue, tmpHash, hash_key_ptr_int, compare_ptr_int);

				currPred->valuesArraySize = 1;
				currPred->values = malloc(sizeof(PredValue) * currPred->valuesArraySize );
				currPred->id = currPred->attribute * DIMENSION_SHIFTING_FACTOR + currPred->operator * OPERATORS_SHIFTING_FACTOR;
				currPred->values[0] = equilikely_inclusive(1, experiment->systemDistinctValue);
				currPred->id += currPred->values[0]/DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR;
				setBoundary(&currPred->valuesBoundary, currPred->values[0], currPred->values[0]);

				if ((oldPred = search_predicate(predicateIndex, currPred)) == NULL){
					if (!insert_predicate(predicateIndex, currPred, currPred)) error("Cannot insert into predicate into  Predicate Index (hashtable)!");
					if(experiment->isEventHashEnabled) insert_HASH(currPred, currPred, currEvent->predicateHash, hash_identity_rpredicate_attr_only, compare_rpredicate_attr_only);
					insert_last_LIST(currPred, currEvent->predicateList);
				}else{
					free(currPred->values);
					free(currPred);
					if(experiment->isEventHashEnabled) insert_HASH(oldPred, oldPred, currEvent->predicateHash, hash_identity_rpredicate_attr_only, compare_rpredicate_attr_only);
					insert_last_LIST(oldPred, currEvent->predicateList);
				}
			}else{
				if(experiment->isInforceEventDim){j--;}
				free(currValue);
				free(currPred);
			}
		}

		destroy_HASH(tmpHash, free, dummy_free);
		insert_last_LIST(currEvent, eventList);
	}
	return eventList;
}



List random_event_uniform(const ExperimentParameter* experiment){
	Event* e;
	Predicate* p;
	int i, j;
	HashTable tmp;
	int* k;
	List eventList = make_empty_LIST( NULL, free );

	if(experiment->systemDim > MAX_NUMBER_OF_DIMENSION)
			fatal_error("The number of requested dimension exceeds the maximum number dimensions allowed by the system!");

	for(i = 0; i < experiment->eventCount; i++ ){
		e = malloc(sizeof(Event));
		e->id = i;
		e->predicateHash = create_HASH( experiment->eventMaxDim );
		tmp = create_HASH( experiment->eventMaxDim );

		for(j = 0; j < experiment->eventMaxDim; j++ ){
			k = malloc(sizeof(int));
			p = malloc(sizeof(Predicate));
			p->id = equilikely(1, experiment->systemDim-1)*(MAX_NUMBER_OF_DIMENSION);
			*k = p->id;

			if(find_HASH(k, tmp, hash_key_ptr_int, compare_ptr_int) == NULL){
				insert_HASH(k, k, tmp, hash_key_ptr_int, compare_ptr_int);
				p->id += equilikely(1, experiment->systemDistinctValue-1);
				insert_HASH(p, p, e->predicateHash, hash_key_ptr_predicate, compare_ptr_predicate);
			}else{
				free(k);
				free(p);
			}
		}

		destroy_HASH(tmp, free, dummy_free);
		insert_last_LIST(e, eventList);
	}
	return eventList;
}


List random_zevent_uniform(const ExperimentParameter* experiment){
	ZEvent* e;
	ZPredicate* p;
	int i, j;
	HashTable tmp;
	int* k;
	List eventList = make_empty_LIST( NULL, free );

	if(experiment->systemDim > MAX_NUMBER_OF_DIMENSION)
			fatal_error("The number of requested dimension exceeds the maximum number dimensions allowed by the system!");

	for(i = 0; i < experiment->eventCount; i++ ){
		e = malloc(sizeof(ZEvent));
		e->id = i;
		e->eventPredicate = make_empty_LIST(NULL, dummy_free);
		tmp = create_HASH( experiment->eventMaxDim );

		for(j = 0; j < experiment->eventMaxDim; j++ ){
			k = malloc(sizeof(int));
			p = malloc(sizeof(ZPredicate));
			p->id = equilikely(1, experiment->systemDim);
			*k = p->id;

			if(find_HASH(k, tmp, hash_key_ptr_int, compare_ptr_int) == NULL){
				insert_HASH(k, k, tmp, hash_key_ptr_int, compare_ptr_int);
				p->value = equilikely(1, experiment->systemDistinctValue);
				insert_last_LIST(p, e->eventPredicate);
			}else{
				free(k);
				free(p);
			}
		}

		destroy_HASH(tmp, free, dummy_free);
		insert_last_LIST(e, eventList);
	}
	return eventList;
}






