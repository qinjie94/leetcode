/*
 * generateWorkload.c
 *
 *  Created on: Nov 5, 2009
 *      Author: Mo Sadoghi
 */


#include "generateWorkLoad.h"



void setSubWorkloadParameter(WorkloadParameter *workloadParameter, const ExperimentParameter* experiment){
	workloadParameter->qgramID  = experiment->qgramID;
	workloadParameter->objectCount  = experiment->subCount;
	workloadParameter->objectMaxDim  = experiment->subMaxDim;
	workloadParameter->objectMinDim  = experiment->subMinDim;
	workloadParameter->objectOleq  = experiment->subOleq;
	workloadParameter->objectOgeq  = experiment->subOgeq;
	workloadParameter->objectOeq  = experiment->subOeq;
	workloadParameter->objectOneq  = experiment->subOneq;
	workloadParameter->objectOlgeq  = experiment->subOlgeq;
	workloadParameter->objectOin  = experiment->subOin;
	workloadParameter->objectOnotin  = experiment->subOnotin;
	workloadParameter->isInforceOeq  = experiment->isInforceOeq;
	workloadParameter->objectOeqRatio  = experiment->subOeqRatio;
	workloadParameter->objectMaxExtendedObjectSize  = experiment->subMaxExtendedObjectSize;
	workloadParameter->objectMaxExtendedObjectSizeUniform  = experiment->subMaxExtendedObjectSizeUniform;
	workloadParameter->objectMaxExtendedObjectSizeZip  = experiment->subMaxExtendedObjectSizeZip;
	workloadParameter->objectMaxExtendedObjectSizeZipAlpha  = experiment->subMaxExtendedObjectSizeZipAlpha;
	workloadParameter->objectHasMinExtendedObjectSize  = experiment->subHasMinExtendedObjectSize;
	workloadParameter->objectMaxNumberInNotIn  = experiment->subMaxNumberInNotIn;
	workloadParameter->objectUniform  = experiment->subUniform;
	workloadParameter->objectZip  = experiment->subZip;
	workloadParameter->objectZipAlpha  = experiment->subZipAlpha;
	workloadParameter->objectUniformModel  = experiment->subUniformModel;
	workloadParameter->objectWorkloadMatchingDegree  = experiment->subWorkloadMatchingDegree;
	workloadParameter->objectIsIncludeBaseTupe  = experiment->subIsIncludeBaseTupe;
	workloadParameter->objectIsRandomOrder  = experiment->subIsRandomOrder;
	workloadParameter->isInforceObjectDim  = experiment->isInforceSubDim;
	strcpy(workloadParameter->objectOutput, experiment->subOutput);
	strcpy(workloadParameter->objectInput, experiment->subInput);
	workloadParameter->systemDim  = experiment->systemDim;
	workloadParameter->systemDimMeanScoreRatio  = experiment->systemDimMeanScoreRatio;
	workloadParameter->systemDimVarianceScoreRatio  = experiment->systemDimVarianceScoreRatio;
	workloadParameter->systemDistinctValue  = experiment->systemDistinctValue;
	workloadParameter->systemMinDistinctValue  = experiment->systemMinDistinctValue;
	workloadParameter->systemMaxDistinctCluster  = experiment->systemMaxDistinctCluster;
	workloadParameter->systemMaxDistinctClusterSize  = experiment->systemMaxDistinctClusterSize;
	workloadParameter->systemDistinctValueUniform  = experiment->systemDistinctValueUniform;
	workloadParameter->systemDistinctValueZip  = experiment->systemDistinctValueZip;
	workloadParameter->systemDistinctValueZipAlpha  = experiment->systemDistinctValueZipAlpha;
	workloadParameter->distinctValueIndex  = experiment->distinctValueIndex;
	workloadParameter->systemObjectOptCount  = experiment->systemSubOptCount;

}



void setEventWorkloadParameter(WorkloadParameter *workloadParameter, const ExperimentParameter* experiment){
	workloadParameter->qgramID  = experiment->qgramID;
	workloadParameter->objectCount  = experiment->eventCount;
	workloadParameter->objectMaxDim  = experiment->eventMaxDim;
	workloadParameter->objectMinDim  = experiment->eventMinDim;
	workloadParameter->objectOleq  = 0;
	workloadParameter->objectOgeq  = 0;
	workloadParameter->objectOeq  = experiment->eventOeq;
	workloadParameter->objectOneq  = 0;
	workloadParameter->objectOlgeq  = 0;
	workloadParameter->objectOin  = 0;
	workloadParameter->objectOnotin  = 0;
	workloadParameter->isInforceOeq  = experiment->isInforceOeq;
	workloadParameter->objectMaxExtendedObjectSize  = 0;
	workloadParameter->objectMaxNumberInNotIn  = 0;
	workloadParameter->objectUniform  = experiment->eventUniform;
	workloadParameter->objectZip  = experiment->eventZip;
	workloadParameter->objectZipAlpha  = experiment->eventZipAlpha;
	workloadParameter->objectUniformModel  = experiment->eventUniformModel;
	workloadParameter->objectWorkloadMatchingDegree  = 0;
	workloadParameter->isInforceObjectDim  = experiment->isInforceSubDim;
	strcpy(workloadParameter->objectOutput, experiment->eventOutput);
	strcpy(workloadParameter->objectInput, experiment->eventInput);
	workloadParameter->systemDim  = experiment->systemDim;
	workloadParameter->systemDimMeanScoreRatio  = experiment->systemDimMeanScoreRatio;
	workloadParameter->systemDimVarianceScoreRatio  = experiment->systemDimVarianceScoreRatio;
	workloadParameter->systemDistinctValue  = experiment->systemDistinctValue;
	workloadParameter->systemMinDistinctValue  = experiment->systemMinDistinctValue;
	workloadParameter->systemMaxDistinctCluster  = experiment->systemMaxDistinctCluster;
	workloadParameter->systemMaxDistinctClusterSize  = experiment->systemMaxDistinctClusterSize;
	workloadParameter->systemDistinctValueUniform  = experiment->systemDistinctValueUniform;
	workloadParameter->systemDistinctValueZip  = experiment->systemDistinctValueZip;
	workloadParameter->systemDistinctValueZipAlpha  = experiment->systemDistinctValueZipAlpha;
	workloadParameter->distinctValueIndex  = experiment->distinctValueIndex;
	workloadParameter->systemObjectOptCount  = 1;
}


void generate_workload(ExperimentParameter *experimentParameter) {
	List randomSub;
	List randomEvent;
	List randomBase;
	clock_t start,end;
	long double dif;
	int maxNumObj;

	WorkloadParameter subWorkloadParameter, eventWorkloadParameter, baseWorkloadParameter;
	WorkloadStat subWorkloadStat, eventWorkloadStat, baseWorkloadStat;

	struct hashtable *predicateIndex;

	int isPrintStructure, isPrintSub, isPrintEvent, isManaualRemove;

	start = clock();


	ini_workload_stat(&subWorkloadStat);
	ini_workload_stat(&eventWorkloadStat);
	ini_workload_stat(&baseWorkloadStat);


	setSubWorkloadParameter(&subWorkloadParameter, experimentParameter);
	setEventWorkloadParameter(&eventWorkloadParameter, experimentParameter);
	setEventWorkloadParameter(&baseWorkloadParameter, experimentParameter);

	isPrintStructure = FALSE;
	isManaualRemove = FALSE;
	isPrintSub = FALSE;
	isPrintEvent = FALSE;
	randomBase = NULL;

	predicateIndex = create_hashtable(MAX_NUMBER_OF_DIMENSION, hash_thomas_wang_rpredicate, equal_rpredicate_keys);

	if(experimentParameter->subMinDim > experimentParameter->eventMinDim)
		error("eventMinDim must be greater than subMinDim");

	if(experimentParameter->subMaxDim > experimentParameter->eventMaxDim)
		error("eventMaxDim must be greater than subMaxDim");

	if(experimentParameter->eventZip != experimentParameter->subZip)
		fatal_error("both event and subscription must follow a similar distributions");

	if(experimentParameter->eventZip == experimentParameter->subZip){
		if(experimentParameter->eventZipAlpha != experimentParameter->subZipAlpha){
			fatal_error("both event and subscription must follow alpha distributions values");
		}
	}



	if(experimentParameter->profileID == PROFILE_WORKLOAD_RANDOM_DATA || experimentParameter->profileID == PROFILE_WORKLOAD_RANDOM_SUB_RANDOM_SUBSET_EVENT){
		experimentParameter->distinctValueIndex = malloc(sizeof(int) * experimentParameter->systemDim + 1);
		experimentParameter->distinctClusterValueIndex = malloc(sizeof(int*) * experimentParameter->systemDim + 1);
		experimentParameter->distinctClusterValueIndexSize = malloc(sizeof(int) * experimentParameter->systemDim + 1);
		ini_predvalue_array(experimentParameter->distinctValueIndex, experimentParameter->systemDim + 1);
		ini_2d_predvalue_array_null(experimentParameter->distinctClusterValueIndex, experimentParameter->systemDim + 1);
		ini_predvalue_array(experimentParameter->distinctClusterValueIndexSize, experimentParameter->systemDim + 1);
		subWorkloadParameter.distinctValueIndex = experimentParameter->distinctValueIndex;
		eventWorkloadParameter.distinctValueIndex = experimentParameter->distinctValueIndex;
		subWorkloadParameter.distinctClusterValueIndex = experimentParameter->distinctClusterValueIndex;
		eventWorkloadParameter.distinctClusterValueIndex = experimentParameter->distinctClusterValueIndex;
		subWorkloadParameter.distinctClusterValueIndexSize = experimentParameter->distinctClusterValueIndexSize;
		eventWorkloadParameter.distinctClusterValueIndexSize = experimentParameter->distinctClusterValueIndexSize;


		if(experimentParameter->profileID == PROFILE_WORKLOAD_RANDOM_DATA){
			randomSub = random_object_advance(&subWorkloadParameter, &subWorkloadStat, predicateIndex);
			randomEvent = random_object_advance(&eventWorkloadParameter, &eventWorkloadStat, predicateIndex);
		}else{
			randomSub = random_object_advance(&subWorkloadParameter, &subWorkloadStat, predicateIndex);
			if(subWorkloadStat.eqObjectCount == 0)
				fatal_error("The subscription workload must have equality objects to generate event as a subset of subscriptions!");
			randomEvent = get_random_subset_by_clonning(randomSub, &eventWorkloadParameter, &eventWorkloadStat, (float) eventWorkloadParameter.objectCount / (float) subWorkloadStat.eqObjectCount, TRUE);
		}

	}else if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA ||
			 experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE ||
			 experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE ||
			 experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_EQUALITY_RAW_DATA ||
			 experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA ||
			 experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_RESTRICTED_SIZE ||
			 experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_REDUCED_SIZE ||
			 experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RANDOM_DATA){

		if(experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RANDOM_DATA){
			experimentParameter->distinctValueIndex = malloc(sizeof(int) * experimentParameter->systemDim + 1);
			experimentParameter->distinctClusterValueIndex = malloc(sizeof(int*) * experimentParameter->systemDim + 1);
			experimentParameter->distinctClusterValueIndexSize = malloc(sizeof(int) * experimentParameter->systemDim + 1);
			ini_predvalue_array(experimentParameter->distinctValueIndex, experimentParameter->systemDim + 1);
			ini_2d_predvalue_array_null(experimentParameter->distinctClusterValueIndex, experimentParameter->systemDim + 1);
			ini_predvalue_array(experimentParameter->distinctClusterValueIndexSize, experimentParameter->systemDim + 1);
			subWorkloadParameter.distinctValueIndex = experimentParameter->distinctValueIndex;
			eventWorkloadParameter.distinctValueIndex = experimentParameter->distinctValueIndex;
			subWorkloadParameter.distinctClusterValueIndex = experimentParameter->distinctClusterValueIndex;
			eventWorkloadParameter.distinctClusterValueIndex = experimentParameter->distinctClusterValueIndex;
			subWorkloadParameter.distinctClusterValueIndexSize = experimentParameter->distinctClusterValueIndexSize;
			eventWorkloadParameter.distinctClusterValueIndexSize = experimentParameter->distinctClusterValueIndexSize;


			eventWorkloadParameter.objectCount = 1 / (subWorkloadParameter.objectWorkloadMatchingDegree);
			randomSub = random_object_advance(&eventWorkloadParameter, &eventWorkloadStat, predicateIndex);

			eventWorkloadParameter.objectCount = experimentParameter->eventCount;
			subWorkloadParameter.systemDistinctValue = eventWorkloadParameter.systemDistinctValue;
			subWorkloadParameter.systemDim = eventWorkloadParameter.systemDim;

		}else if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA){
			randomSub = generate_qgram_workload(&subWorkloadParameter, &subWorkloadStat, predicateIndex);

		}else if(experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_EQUALITY_RAW_DATA ||
				 experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA){

			if(experimentParameter->subWorkloadMatchingDegree == 0){
				randomSub = generate_qgram_workload(&subWorkloadParameter, &subWorkloadStat, predicateIndex);
			}else{
				randomSub = generate_qgram_workload_minsize(&subWorkloadParameter, &subWorkloadStat, predicateIndex, (eventWorkloadParameter.objectMinDim + eventWorkloadParameter.objectMaxDim)/2);
			}

		}else if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE ||
		   experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_RESTRICTED_SIZE){
			randomSub = generate_qgram_workload_minsize_maxsize(&subWorkloadParameter, &subWorkloadStat, predicateIndex, eventWorkloadParameter.objectMinDim, eventWorkloadParameter.objectMaxDim);

		}else if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE ||
				experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_REDUCED_SIZE ){
			randomSub = generate_qgram_workload_minsize(&subWorkloadParameter, &subWorkloadStat, predicateIndex, (eventWorkloadParameter.objectMinDim + eventWorkloadParameter.objectMaxDim)/2);
		}

		if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA ||
		   experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE ||
		   experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE){

			if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA){
				randomEvent = get_random_subset_by_clonning(randomSub, &eventWorkloadParameter, &eventWorkloadStat, (float) eventWorkloadParameter.objectCount / (float) subWorkloadStat.eqObjectCount, TRUE);
			/* if restricted option is used, then randomly drop some of the subscription elements */
			}else if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE){
				randomEvent = get_random_subset_by_clonning(randomSub, &eventWorkloadParameter, &eventWorkloadStat, (float) eventWorkloadParameter.objectCount / (float) subWorkloadStat.eqObjectCount, TRUE);
				subWorkloadParameter.objectMaxDim = experimentParameter->subMaxDim;
				subWorkloadParameter.objectMinDim = experimentParameter->subMinDim;
				drop_random_dim(randomSub, &subWorkloadParameter, &subWorkloadStat);
			}else if(experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE){

				/* Fix the events */
				subWorkloadParameter.objectMaxDim = experimentParameter->eventMaxDim;
				subWorkloadParameter.objectMinDim = experimentParameter->eventMinDim;
				drop_random_dim(randomSub, &subWorkloadParameter, &subWorkloadStat);

				/* Get a random event after fixing the size */
				randomEvent = get_random_subset_by_clonning(randomSub, &eventWorkloadParameter, &eventWorkloadStat, (float) eventWorkloadParameter.objectCount / (float) subWorkloadStat.eqObjectCount, TRUE);

				/* Fix the subscriptions */
				subWorkloadParameter.objectMaxDim = experimentParameter->subMaxDim;
				subWorkloadParameter.objectMinDim = experimentParameter->subMinDim;
				drop_random_dim(randomSub, &subWorkloadParameter, &subWorkloadStat);
			}

			eventWorkloadParameter.systemDistinctValue = subWorkloadParameter.systemDistinctValue;
			eventWorkloadParameter.systemDim = subWorkloadParameter.systemDim;
			eventWorkloadParameter.distinctValueIndex = subWorkloadParameter.distinctValueIndex;

		}else{
			maxNumObj = get_size_LIST(randomSub);
			/* Randomly remove sub only for real data sets because we want to read the entire data first, then remove some.
			 * Also for randomly generated data, we generate a correct number of objects with correct distributions so adjustment is necessary. */
			if(experimentParameter->profileID != PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RANDOM_DATA){
				remove_random_subset(randomSub, &subWorkloadParameter, ((float) 1/(experimentParameter->subWorkloadMatchingDegree)));
			}

			/* Fix the events (base objects) */
			if(experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_REDUCED_SIZE){
				subWorkloadParameter.objectMaxDim = experimentParameter->eventMaxDim;
				subWorkloadParameter.objectMinDim = experimentParameter->eventMinDim;
				drop_random_dim(randomSub, &subWorkloadParameter, &subWorkloadStat);
			}

			ini_workload_stat(&eventWorkloadStat);
			randomEvent = get_random_subset_by_clonning(randomSub, &eventWorkloadParameter, &eventWorkloadStat, (float) eventWorkloadParameter.objectCount / (float) get_size_LIST(randomSub), TRUE);
			randomBase = get_random_subset_by_clonning(randomSub, &baseWorkloadParameter, &baseWorkloadStat, ALL, TRUE);

			ini_workload_stat(&subWorkloadStat);
			subWorkloadParameter.objectMaxDim = experimentParameter->subMaxDim;
			subWorkloadParameter.objectMinDim = experimentParameter->subMinDim;

			if(experimentParameter->profileID != PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RANDOM_DATA)
				subWorkloadParameter.objectCount = experimentParameter->subCount;


			experimentParameter->idfIndex = NULL;
			/* if derived predicate distribution is non-uniform and most follow from the base predicate, then we need to construct score of the base predicates */
			if(experimentParameter->dataDistributionType == DERIVED_OBJECTS_SELECTED_BASED_ON_ORIGINAL_DISTRIBUTIONS){
				experimentParameter->idfIndex = malloc(sizeof(PredScore) * subWorkloadParameter.systemDim + 1);
				ini_predscore_array(experimentParameter->idfIndex, subWorkloadParameter.systemDim + 1);
				compute_workload_attribute_frequency(randomBase, experimentParameter->idfIndex);
				compute_idf_score(experimentParameter->idfIndex, subWorkloadParameter.systemDim + 1, subWorkloadParameter.objectCount);
			}


			if(experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_EQUALITY_RAW_DATA){
				generate_matching_probability_equality_only(randomSub, &subWorkloadParameter, &subWorkloadStat, subWorkloadParameter.objectWorkloadMatchingDegree * subWorkloadParameter.objectCount, maxNumObj + 1);
			}else if(experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA ||
					experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_RESTRICTED_SIZE ||
					experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_REDUCED_SIZE){
				generate_matching_probability(randomSub, experimentParameter, &subWorkloadParameter, &subWorkloadStat, subWorkloadParameter.objectWorkloadMatchingDegree * subWorkloadParameter.objectCount, maxNumObj + 1, predicateIndex);
			}else if(experimentParameter->profileID == PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RANDOM_DATA){
				generate_matching_probability(randomSub, experimentParameter, &subWorkloadParameter, &subWorkloadStat, subWorkloadParameter.objectWorkloadMatchingDegree * subWorkloadParameter.objectCount, maxNumObj + 1, predicateIndex);
			}

			if(subWorkloadParameter.objectIsRandomOrder){
				randomSub = scramble_objects_list(randomSub);
			}
		}

		eventWorkloadParameter.systemDistinctValue = subWorkloadParameter.systemDistinctValue;
		eventWorkloadParameter.systemDim = subWorkloadParameter.systemDim;
		eventWorkloadParameter.distinctValueIndex = subWorkloadParameter.distinctValueIndex;
	}


	/* Scoring begins either using based idf scheme seeded by either (attr) or (attr, value) */
	subWorkloadParameter.idfIndex = NULL;
	eventWorkloadParameter.idfIndex = NULL;
	experimentParameter->idfHashIndex = NULL;
	subWorkloadParameter.idfHashIndex = NULL;
	eventWorkloadParameter.idfHashIndex = NULL;

	experimentParameter->idfHashIndex = create_hashtable(MAX_NUMBER_OF_DIMENSION, hash_thomas_wang_attr_value, compare_attr_value);
	subWorkloadParameter.idfHashIndex = experimentParameter->idfHashIndex;
	eventWorkloadParameter.idfHashIndex = experimentParameter->idfHashIndex;

	/* Create attribute frequency */
	if(experimentParameter->profileID == PROFILE_WORKLOAD_RANDOM_DATA ||
	   experimentParameter->profileID == PROFILE_WORKLOAD_RANDOM_SUB_RANDOM_SUBSET_EVENT ||
	   experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA ||
	   experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE ||
	   experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE){

		compute_workload_attribute_frequency_hash(randomSub, experimentParameter->idfHashIndex, experimentParameter->scoringType, FALSE);

		if(experimentParameter->profileID == PROFILE_WORKLOAD_RANDOM_DATA ||
		   experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE ||
		   experimentParameter->profileID == PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE)
			compute_workload_attribute_frequency_hash(randomEvent, experimentParameter->idfHashIndex, experimentParameter->scoringType, TRUE);
	}else{
		compute_workload_attribute_frequency_hash(randomBase, experimentParameter->idfHashIndex, experimentParameter->scoringType, TRUE);
	}

	/* Compute idf scores for both sub/events */
	compute_idf_score_hash(experimentParameter->idfHashIndex, subWorkloadParameter.objectCount);
	compute_object_pred_score_hash(randomSub, &subWorkloadParameter, experimentParameter->scoringType);
	compute_object_pred_score_hash(randomEvent, &eventWorkloadParameter, experimentParameter->scoringType);



	if(experimentParameter->matchingType == MATCHING_TYPE_STABBING_SUBSCRIPTION_EXPRESSIVE_EVENT ||
	   experimentParameter->matchingType == MATCHING_TYPE_SYMMETRIC_MATCHING_EXPRESSIVE_EVENT){
		eventWorkloadParameter.objectOeq = FALSE;
		eventWorkloadParameter.objectOeqRatio = 0;
		eventWorkloadParameter.objectOgeq = TRUE;
		eventWorkloadParameter.objectOleq = TRUE;
		eventWorkloadParameter.objectOlgeq = TRUE;
		eventWorkloadParameter.objectOin = FALSE;
		eventWorkloadParameter.objectHasMinExtendedObjectSize = subWorkloadParameter.objectHasMinExtendedObjectSize;
		eventWorkloadParameter.objectMaxExtendedObjectSize = subWorkloadParameter.objectMaxExtendedObjectSize;
		eventWorkloadParameter.objectMaxNumberInNotIn = subWorkloadParameter.objectMaxNumberInNotIn;
		eventWorkloadParameter.systemObjectOptCount  = subWorkloadParameter.systemObjectOptCount;
		/* eventWorkloadParameter.isEqualityRanking = FALSE; */
		eventWorkloadParameter.isInforceOeq = FALSE;
		eventWorkloadParameter.objectIsIncludeBaseTupe = FALSE;
		eventWorkloadParameter.distinctValueIndex = subWorkloadParameter.distinctValueIndex;
		ini_workload_stat(&eventWorkloadStat);
		generate_matching_probability(randomEvent, experimentParameter, &eventWorkloadParameter, &eventWorkloadStat, 0, maxNumObj + 1 + get_size_LIST(randomSub) + 1, predicateIndex);

		eventWorkloadParameter.systemDistinctValue = subWorkloadParameter.systemDistinctValue;
		eventWorkloadParameter.systemDim = subWorkloadParameter.systemDim;
		eventWorkloadParameter.distinctValueIndex = subWorkloadParameter.distinctValueIndex;
	}


	write_list_object_to_file(&subWorkloadParameter, &subWorkloadStat, randomSub);
	write_list_object_to_file(&eventWorkloadParameter, &eventWorkloadStat, randomEvent);


	if(isPrintSub) print_list_object(randomSub, 10);
	if(isPrintSub) puts("\n\n");
	if(isPrintEvent) print_list_object(randomEvent, 10);


	randomSub = delete_list_LIST(randomSub, delete_ptr_object_nopred);
	randomEvent = delete_list_LIST(randomEvent, delete_ptr_object_nopred);
	if(randomBase != NULL) randomBase = delete_list_LIST(randomBase, delete_ptr_object_nopred);


	hashtable_destroy(predicateIndex, FALSE, delete_ptr_rpredicate);
	predicateIndex = NULL;

	if(experimentParameter->dataDistributionType == DERIVED_OBJECTS_SELECTED_BASED_ON_ORIGINAL_DISTRIBUTIONS){
		free(experimentParameter->idfIndex);
		experimentParameter->idfIndex = NULL;
	}

	hashtable_destroy(experimentParameter->idfHashIndex, TRUE, free);
	experimentParameter->idfHashIndex = NULL;

	end = clock();
	dif = (end - start) / (float) (CLOCKS_PER_SEC);
	printf ("Total time: %Lf.\n", dif );
}



