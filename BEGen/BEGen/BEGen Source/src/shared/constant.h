/*
 * constant.h
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 */


#ifndef _Constant_h
#define _Constant_h

//#define NDEBUG


#define PARTITIONING_ENABLED 1
#define PARTITIONING_DISABLED 0
#define VERIFY_CORRECTNESS 1
#define NOT_VERIFY_CORRECTNESS 0
#define ROOT_CLUSTER 0
#define CLUSTER_START_VALUE 1
#define INITIAL_CLUSTER 0
#define EMPTY -1
#define ALL 1
#define TRUE 1
#define FALSE 0
#define PASSED 1
#define FAILED 0
#define MAX_QUEUE_SIZE_PRINTING 2000
#define MAX_BUFFER_SIZE_FOR_PRINTING 2000
#define MAX_BUFFER_SIZE_FOR_CONVERSION 10
#define DECIMAL_BASE 10
#define MIN_INT (-32768)
#define MAX_INT (32768)
#define DECIMAL_BASE 10


#define MAX_LEAF_CAPACITY 10
#define HASHTABLE_LOAD_FACTOR 1.3
#define THETA_PREDICATE_RATIO 0.5
#define SPLIT_TOLERANCE_LEVEL 0.25
#define LEAF_SIZE_EXPANSION_FACTOR 1.5
#define MAX_DISTINCT_PREDICATE_COUNT 1000
#define APPROX_DISTINCT_PREDICATE_COUNT_PRE_CLUSTER 100


#define MAX_NUMBER_ARRAY_VALUE_SIZE 250

#define MAX_NUMBER_OF_DIMENSION 65000
#define MAX_NUMBER_OF_DISTINCT_VALUES_DIMENSION 2500000000
#define MIN_NUMBER_OF_DISTINCT_VALUES_DIMENSION 2
#define MAX_NUMBER_OF_OPERATORS 10


#define DIMENSION_SHIFTING_FACTOR 9831
#define DISTINCT_VALUES_DIMENSION_SHIFTING_FACTOR 193
#define DISTINCT_VALUES_DIMENSION_REDUCTION_FACTOR 19
#define OPERATORS_SHIFTING_FACTOR 19


#define MIN_NUMBER_OF_CLUSTERS 33
#define MIN_NUMBER_OF_PARTITION 1001
#define MIN_CLUSTER_CAPACITY 10
#define MAX_CLUSTER_CAPACITY 20

#define OPT_LEVEL_0 0
#define OPT_ELIMINATE_PREDICATE 1
#define OPT_CONSUMED_SUBSCRIPTION 2
#define OPT_LEVEL_3 3

#define SCORE_MOST_POPULAR 0
#define SCORE_LEAST_POPULAR 1
#define SCORE_MOST_SELECTIVE 2
#define SCORE_MOST_SELECTIVE_WORST_CASE 3


#define OPTIMAL_SPLITTING_ALGORITHM 0
#define APPROXIMATE_SPLITTING_ALGORITHM 1


#define EMPTY_STRING ""


#define OPT_EQ 1
#define OPT_NEQ 2
#define OPT_LEQ 3
#define OPT_GEQ 4
#define OPT_LGEQ 5
#define OPT_IN 6
#define OPT_NOTIN 7


#define RANK_OPT_EQ 1
#define RANK_OPT_IN 2
#define RANK_OPT_LGEQ 3
#define RANK_OPT_LEQ 4
#define RANK_OPT_GEQ 4
#define RANK_OPT_NOTIN 5
#define RANK_OPT_NEQ 6


#define VARIANCE_IGNORE 0
#define VARIANCE_START 1
#define VARIANCE_MIDDLE 2
#define VARIANCE_END 3

#define NULL_VALUE 0

#define MIN_PARTITION_WINDOW_SIZE 20
#define MIN_CLUSTER_WINDOW_SIZE 20



#define PROFILE_WORKLOAD_RANDOM_DATA 0
#define PROFILE_WORKLOAD_RANDOM_SUB_RANDOM_SUBSET_EVENT 1
#define PROFILE_WORKLOAD_RAW_DATA 2
#define PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE 3
#define PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE 4
#define PROFILE_WORKLOAD_MATCHING_PROBABILITY_EQUALITY_RAW_DATA 5
#define PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA 6
#define PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_RESTRICTED_SIZE 7
#define PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_REDUCED_SIZE 8
#define PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RANDOM_DATA 9

#define QGRAM_BASE_THREE_GRAM 0
#define QGRAM_THREE_GRAM_EXTENDED_DOMAIN_SIZE 1



#define MATCHING_TYPE_STABBING_SUBSCRIPTION_EQUALITY_ONLY_EVENT 0
#define MATCHING_TYPE_STABBING_SUBSCRIPTION_EXPRESSIVE_EVENT 1
#define MATCHING_TYPE_SYMMETRIC_MATCHING_EXPRESSIVE_EVENT 2


#define SCORING_ATTRIBUTE_FREQUENCY_INDEX_BASED_DEPRECATED -1
#define SCORING_ATTRIBUTE_FREQUENCY 0
#define SCORING_ATTRIBUTE_VALUE_FREQUENCY 1

#define DERIVED_OBJECTS_SELECTED_RANDOMLY 0
#define DERIVED_OBJECTS_SELECTED_BASED_ON_ORIGINAL_DISTRIBUTIONS 1


#define HASH_FUNC_NAIVE 0
#define HASH_FUNC_ROBERT_JENKINS 1
#define HASH_FUNC_THOMAS_WONG 2

#define MIN_HEAP_SIZE_FOR_PNODE_BETREE 10000

#define BYTE_TO_KBYTE_CONVERSION 1024
#define KBYTE_TO_MGBYTE_CONVERSION 1024

#define USE_NONE_INACTIVE_DIMENSION_FOR_SEARCH -1
#define USE_ALL_INACTIVE_DIMENSION_FOR_SEARCH 0

#endif



