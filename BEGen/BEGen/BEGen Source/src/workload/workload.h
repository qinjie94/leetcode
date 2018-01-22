/*
 * workload.h
 *
 *  Created on: May 21, 2009
 *      Author: Mo Sadoghi
 */

#ifndef WORKLOAD_H_
#define WORKLOAD_H_

#include <ctype.h>
#include <time.h>
#include "../sort/quicksort.h"
#include "../shared/generic.h"
#include "../shared/clock.h"
#include "../shared/timer.h"
#include "../randgen/randgen.h"
#include "../hashtable/hashtable.h"
#include "../hashtable/hashtable_itr.h"


void compute_workload_attribute_frequency(List objList, PredScore *idfIndex);
void compute_workload_attribute_frequency_hash(List objList, struct hashtable *idfHashIndex, int scoringType, int isControlledMatching);
void compute_idf_score(PredScore *idfIndex, int dimCount, int numberOfObject);
void compute_idf_score_hash(struct hashtable *idfHashIndex, int numberOfObject);
void compute_object_pred_score(List randomSub, const WorkloadParameter* workload);
void compute_object_pred_score_hash(List objectList, const WorkloadParameter* workload, int scoringType);

List random_sub_uniform(const ExperimentParameter* experiment, WorkloadStat* workloadStat, int isInforceSubDim);
List random_zsub_uniform(const ExperimentParameter* experiment, WorkloadStat* workloadStat);
List random_object_advance(const WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex);
List get_random_subset(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, double selectionProbability, int isEqualityOnly);
List get_random_subset_by_clonning(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, double selectionProbability, int isEqualityOnly);
List get_random_subset_minsize(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, uint32_t minSize);
List get_random_subset_minsize_maxsize(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, int minSize, int maxSize);
List reduce_dim(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat);
void drop_random_dim(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat);
void index_objects(List objList);
void remove_random_subset(List objList, WorkloadParameter* workload, double minNumObject);
void generate_matching_probability(List objList, ExperimentParameter *experimentParameter, WorkloadParameter* workload, WorkloadStat* workloadStat, int duplicationDegree, int minID, struct hashtable *predicateIndex);
void generate_matching_probability_equality_only(List objList, WorkloadParameter* workload, WorkloadStat* workloadStat, int duplicationDegree, int minID);
List random_event_uniform(const ExperimentParameter* experiment);
List random_event_rpredicate_uniform(const ExperimentParameter* experiment, struct hashtable *predicateIndex);
List random_zevent_uniform(const ExperimentParameter* experiment);
void print_tree_summary(const TreeStats* treeStat, const ExecutionStats* executionStats, const ExperimentParameter* experiment, const WorkloadStat* workloadStat, const ClusterStats* clusterStats, const TreeParameter *treePram);
void print_cluster_summary_deprecated(const ExecutionStats* executionStats, const ExperimentParameter* experiment, const WorkloadStat* workloadStat, const DataStructureParameter *clusterPram);
void aggregate_tree_summary(GlobalStats* globalStats, const TreeStats* treeStat, const ExecutionStats* executionStats, const ExperimentParameter* experiment, const ClusterStats* clusterStats);
void aggregate_cluster_summary(GlobalStats* globalStats, const ExecutionStats* executionStats, const ExperimentParameter* experiment);


void print_object_id(List subList, int objID);
void print_list_object(List subList, int count);
void print_list_single_object(List subList, int count);
void print_list_event(List subList, int count);


void write_list_object_to_file(const WorkloadParameter* workload, WorkloadStat* workloadStat, List objectList);
List read_object_to_list(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex, int isEnabledDistinctIndexIdfIndexCreation);
List generate_qgram_workload(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex);
List generate_qgram_workload_minsize(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex, int minDimSize);
List generate_qgram_workload_minsize_maxsize(WorkloadParameter* workload, WorkloadStat* workloadStat, struct hashtable *predicateIndex, int minDimSize, int maxDimSize);
List scramble_objects_list(List objList);
void scramble_obj_pred_list(List objList);


/* sig_create_workload_function_signature_cluster(ClusterPropagation) */

#endif /* WORKLOAD_H_ */

