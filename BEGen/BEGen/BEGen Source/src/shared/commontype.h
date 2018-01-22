/*
 * constant.h
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 */
#include "../hash/hashsep.h"
#include "../linkedlist/list.h"
#include "../binaryheap/binheap.h"
#include "stdint.h"

#ifndef _Constanttypes_h
#define _Constanttypes_h

typedef struct boundary Boundary;
typedef struct boundary_stream BoundaryStream;

typedef struct predicate_type Predicate;
typedef struct zipped_predicate_type ZPredicate;
typedef struct rich_predicate_type RPredicate;
typedef struct rich_predicate_score_wrapper RPredicateScorePair;
typedef struct rich_predicate_index RPredicateIndex;
typedef struct rich_predicate_type_stream RPredicateStream;
typedef struct attribute_value_pair AttrValuePair;

typedef struct subscription_type Sub;
typedef struct zipped_subscription_type ZSub;
typedef struct array_object_type Object;
typedef struct matched_object TopkObject;
typedef struct array_object_type_stream ObjectStream;
typedef struct event_type Event;
typedef struct zipped_event_type ZEvent;

typedef struct event_stat EventStats;
typedef struct execution_stat ExecutionStats;
typedef struct tree_stat TreeStats;
typedef struct workload_parameter WorkloadParameter;
typedef struct experiment_parameter ExperimentParameter;
typedef struct workload_stat WorkloadStat;
typedef struct clusert_stat ClusterStats;
typedef struct tree_parameter TreeParameter;
typedef struct data_structure_parameter DataStructureParameter;
typedef struct aggragated_global_stat GlobalStats;


struct boundary{
	PredValue minBoundary;							/* Minimum boundary */
	PredValue maxBoundary;							/* Maximum boundary */
};


struct boundary_stream{
	uint32_t minBoundary;							/* Minimum boundary */
	uint32_t maxBoundary;							/* Maximum boundary */
};


struct event_type{
	int id;
	HashTable predicateHash;
	List predicateList;
};


struct zipped_event_type{
	int id;
	List eventPredicate;
};


struct subscription_type{
	uint32_t id;				/* Each subscription has a unique id associated to it which is used for fast lookup */
	HashTable subPredicate;		/* Holds the list of predicates associated to each subscription for O(1) access */
	List predicate;				/* Holds the list of predicates associated to each subscription in a linked list for sequential access */
};


struct zipped_subscription_type{
	uint32_t id;				/* Each subscription has a unique id associated to it which is used for fast lookup */
	List predicate;				/* Holds the list of predicates associated to each subscription in a linked list for sequential access */
};


/* A wrapper for indexing object attributes which also contains the pointer to attribute score */
struct rich_predicate_index{
	RPredicate *predicate;
	uint8_t		scoreIndex;
};


/* A wrapper for indexing object attributes which also contains the pointer to attribute together with its score */
struct rich_predicate_score_wrapper{
	RPredicate *predicate;
	PredScore	score;
};


/* A wrapper for object for top-k processing use in the min-heap-k */
struct matched_object{
	Object *object;
	PredScore score;
};


struct attribute_value_pair{
	uint16_t attribute;
	PredValue  value;
};

/* If ASub changes, then clone_asubscription must be updated accordingly */
struct array_object_type{
	uint32_t 			id;				/* Each subscription has a unique id associated to it which is used for fast lookup */
	RPredicate 			**predicate;	/* Holds the list of predicates associated to each subscription in a linked list for sequential access */
	uint8_t				*active;
	uint8_t				*covered;
	uint8_t				arraySize;
	uint8_t				numEqualityLeft;
	struct hashtable 	*index;
	PredScore			*score;
};


struct array_object_type_stream{
	uint32_t 			id;				/* Each subscription has a unique id associated to it which is used for fast lookup */
	RPredicateStream 	**predicate;	/* Holds the list of predicates associated to each subscription in a linked list for sequential access */
	uint32_t			*active;
	uint32_t			*covered;
	uint32_t			arraySize;
	uint32_t			numEqualityLeft;
	PredScore			*score;
};



struct predicate_type{
	uint32_t  id;
	//char attributeName;
	//char *operator;
	//char *value;
};


struct zipped_predicate_type{
	uint16_t id;
	uint8_t  operator;
	PredValue  value;
};


struct rich_predicate_type{
	uint32_t id;
	uint16_t attribute;
	uint8_t  operator;
	PredValue  *values;
	uint8_t  valuesArraySize;
	struct boundary valuesBoundary;
	RPredicate		*baseRPredicate;
};


struct rich_predicate_type_stream{
	uint32_t id;
	uint32_t attribute;
	uint32_t operator;
	uint32_t *values;
	uint32_t valuesArraySize;
	struct boundary_stream valuesBoundary;
};


struct experiment_parameter{
	int profileID;
	int qgramID;
	int isRand;
	char argument[8000];

	int subCount;
	int subMaxDim;
	int subMinDim;
	char subOleq;
	char subOgeq;
	char subOeq;
	char subOneq;
	char subOlgeq;
	char subOin;
	char subOnotin;
	char isInforceOeq;
	double subOeqRatio;
	int subMaxExtendedObjectSize;
	int subMaxExtendedObjectSizeUniform;
	int subMaxExtendedObjectSizeZip;
	int subMaxExtendedObjectSizeZipAlpha;
	int subHasMinExtendedObjectSize;
	int subMaxNumberInNotIn;
	char subUniform;
	char subZip;
	int subZipAlpha;
	char subUniformModel;
	double subWorkloadMatchingDegree;
	int subIsIncludeBaseTupe;
	int subIsRandomOrder;
	char isInforceSubDim;
	char subOutput[500];
	char subInput[500];

	int eventCount;
	int eventMaxDim;
	int eventMinDim;
	char eventOeq;
	char eventUniform;
	char eventZip;
	int eventZipAlpha;
	char eventUniformModel;
	char eventChosenFromSub;
	char isInforceEventDim;
	int isEventHashEnabled;
	char eventOutput[500];
	char eventInput[500];
	int	matchingType;

	int systemDim;
	double systemDimMeanScoreRatio;
	double systemDimVarianceScoreRatio;
	PredValue systemDistinctValue;
	PredValue systemMinDistinctValue;
	int systemMaxDistinctCluster;
	int systemMaxDistinctClusterSize;
	int systemDistinctValueUniform;
	int systemDistinctValueZip;
	int systemDistinctValueZipAlpha;
	int systemSubOptCount;


	PredScore *idfIndex;
	struct hashtable *idfHashIndex;


	PredValue *distinctValueIndex;
	PredValue **distinctClusterValueIndex;
	PredValue *distinctClusterValueIndexSize;

	int scoringType;
	int dataDistributionType;

	int hashFunc;
};



struct workload_parameter{
	int qgramID;
	int objectCount;
	int objectMaxDim;
	int objectMinDim;
	char objectOleq;
	char objectOgeq;
	char objectOeq;
	char objectOneq;
	char objectOlgeq;
	char objectOin;
	char objectOnotin;
	char isInforceOeq;
	double objectOeqRatio;
	int objectMaxExtendedObjectSize;
	int objectMaxExtendedObjectSizeUniform;
	int objectMaxExtendedObjectSizeZip;
	int objectMaxExtendedObjectSizeZipAlpha;
	int objectHasMinExtendedObjectSize;
	int objectMaxNumberInNotIn;
	char objectUniform;
	char objectZip;
	int objectZipAlpha;
	char objectUniformModel;
	double objectWorkloadMatchingDegree;
	int objectIsIncludeBaseTupe;
	int objectIsRandomOrder;
	char isInforceObjectDim;
	char objectOutput[500];
	char objectInput[500];

	int systemDim;
	double systemDimMeanScoreRatio;
	double systemDimVarianceScoreRatio;
	PredValue systemDistinctValue;
	PredValue systemMinDistinctValue;
	int systemMaxDistinctCluster;
	int systemMaxDistinctClusterSize;
	int systemDistinctValueUniform;
	int systemDistinctValueZip;
	int systemDistinctValueZipAlpha;
	int systemObjectOptCount;

	PredScore *idfIndex;
	struct hashtable *idfHashIndex;

	PredValue *distinctValueIndex;
	PredValue **distinctClusterValueIndex;
	PredValue *distinctClusterValueIndexSize;

	int	isEqualityRanking;
	int isTopk;
	int topkCount;
	int isTopkNaive;
};

struct workload_stat{
	int eqObjectCount;
	float avgEqOpCount;
	float avgNonEqOpCount;
	float avgObjectDim;
};

#endif



