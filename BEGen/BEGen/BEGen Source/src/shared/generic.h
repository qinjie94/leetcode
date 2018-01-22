/*
 * compare.h
 *
 *  Created on: Apr 29, 2009
 *      Author: Mo Sadoghi
 */

#include "constant.h"
#include "commontype.h"
#include "../shared/fatal.h"
#include "../hashtable/hashtable.h"
#include <stdlib.h>
#include <assert.h>

#ifndef COMPARE_H_
#define COMPARE_H_


void ini_global_stat_tree(GlobalStats* globalStats, TreeParameter* treePram, int subDimCount);
void ini_global_stat_cluster(GlobalStats* globalStats);
void ini_tree_parameter(TreeParameter* treePram);
void ini_cluster_stat(ClusterStats* clusterStats, int arraySize);
void ini_execution_stat(ExecutionStats* executionStats);
void ini_event_stat(EventStats* eventStat, int topkCount);
void ini_predvalue_array(PredValue* array, int arraySize);
void ini_predscore_array(PredScore* array, int arraySize);
void ini_2d_predvalue_array_null(PredValue** array, int arraySize);
void ini_predvalue_array_to_value(PredValue* array, int arraySize, int defaultValue);
inline void ini_uint32_2d_array(uint32_t **array, int row, int col);
inline void ini_longdouble_2d_array(long double **array, int row, int col);
inline void ini_longdouble_1d_array(long double *array, int size);
inline long double** create_longdouble_2d_array(int row, int col);
inline uint32_t** create_uint32_2d_array(uint32_t row, uint32_t col);
inline void free_uint32_2d_array(uint32_t **array, int row, int col);
inline void free_longdoubl_2d_array(long double **array, int row, int col);
void print_2d_array(int **array, int row, int col);
void int_tree_stat(TreeStats* treeStat);
void ini_workload_stat(WorkloadStat* workloadStat);
void ini_experiment_parameter(ExperimentParameter* workloadParameter);
void ini_workload_parameter(WorkloadParameter *workloadParameter);
void ini_tree_stat(TreeStats* treeStat);





void match_event_sub(const Event* event, List subs, List matchedSub, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*));
void match_zevent_zsub(const ZEvent* event, List subs, List matchedSub, int (*compare)(const void*, const void*));
void match_event_object(const Event* event, List subs, List matchedSub, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*));
void match_object_object(const Object* event, List subs, DataStructureParameter *dataStructureParameter, List matchedSubList, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*));
void match_object_object_topk(const Object* event, List subs, DataStructureParameter *dataStructureParameter, PriorityQueue matchedSubMinHeap, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*));
void match_object_object_advance(const Object* event, List subs, List matchedSub, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*));
void match_object_object_advance_topk(const Object* event, List subs, PriorityQueue matchedSubMinHeap, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*));

void delete_ptr_predicate(void* x);
void delete_ptr_zpredicate(void* x);
void delete_ptr_rpredicate(void* x);
void delete_ptr_rpredicateStream(void* x);
void delete_ptr_pointer_list(void* x);
void delete_ptr_clone_subscription(void* x);
void delete_ptr_clone_zsubscription(void* x);
void delete_ptr_subscription(void* x);
void delete_ptr_zsubscription(void* x);

void delete_ptr_object(void* sub);
void delete_ptr_objectStream(void* sub);
void delete_ptr_object_nopred(void* sub);
void delete_ptr_object_with_hash(void* sub);
void delete_ptr_object_nopred_with_hash(void* sub);

void delete_ptr_zevent(void* x);
void delete_ptr_event(void* x);
void delete_ptr_event_nopred(void* x);
void delete_ptr_event_nopred_nohash(void* x);
void delete_ptr_primitive_list(void* x);
void dummy_free(void* x);
void* dummy_clone(void* x);

void* clone_subscription(void* sub);
void* clone_zsubscription(void* sub);
void* clone_object_to_zsub(void* sub);
void* clone_event_to_zevent(void* event);
void* clone_object(void* sub);

void index_object(Object *obj);


int compare_uint16_t(const uint16_t x, const uint16_t y);
int compare_ptr_uint8_t(const void* x, const void* y);
int compare_ptr_int(const void* x, const void* y);
int equal_ptr_int(const void* x, const void* y);
int compare_ptr_float(const void* x, const void* y);
int compare_reverse_ptr_float(const void* x, const void* y);

int compare_ptr_predicate(const void* x, const void* y);
int compare_ptr_zpredicate(const void* x, const void* y);
int compare_ptr_subscription(const void* x, const void* y);
int equal_ptr_subscription(const void* x, const void* y);
int compare_ptr_zsubscription(const void* x, const void* y);
int compare_ptr_object(const void* x, const void* y);
int equal_ptr_object(const void* x, const void* y);


int compare_ptr_zpredicate_dim(const void* x, const void* y);
int compare_ptr_zpredicate_value(const void* x, const void* y);

char* to_string_ptr_int(const void* x);
char* to_string_ptr_float(const void* x);
char* to_string_ptr_subscription(const void* x);
char* to_string_ptr_zsubscription(const void* x);
char* to_string_ptr_predicate(const void* x);
char* to_string_ptr_zpredicate(const void* x);

uint32_t hash_key_ptr_int (const void* x);
uint32_t hash_key_ptr_predicate (const void* x);
uint32_t hash_key_ptr_zpredicate (const void* x);

inline int get_dim_predicate(const void* x);
inline int get_value_predicate(const void* x);

inline int get_dim_zpredicate(const void* x);
inline int get_value_zpredicate(const void* x);

/*
inline uint16_t get_dim_rpredicate(const void* x);
inline uint32_t* get_value_rpredicate(const void* x);
*/



float* get_min_float_value();
float* get_max_float_value();
int* get_min_int_value();
int* get_max_int_value();


void remove_predicate_subscription(Sub* sub, Predicate* pred, uint32_t (*hash_key)(const void*), int (*compare)(const void*, const void*));
void remove_predicate_zsubscription(ZSub* sub, ZPredicate* pred, int (*compare)(const void*, const void*));

int is_equal_str_keys(const void *k1, const void *k2);
int compare_str_keys(const void *k1, const void *k2);
int compare_rpredicate_keys(const void *k1, const void *k2);
int compare_rpredicate_attr_only(const void *k1, const void *k2);
int equal_rpredicate_keys(const void *k1, const void *k2);
int compare_attr_value(const void *k1, const void *k2);
int is_equal_1d_array(const int *array1, const int *array2, int arraySize);
int is_equal_zsub_rsub(const void *zsub, const void *asub);
int is_equal_object_object(const void *object1, const void *object2);
int is_equal_zevent_event(const void *zevent, const void *event);
int equal_uint32_keys(const void *k1, const void *k2);
int equal_obj_keys(const void *k1, const void *k2);
int equal_uint16_keys(const void *k1, const void *k2);
int equal_memory_location(const void *k1, const void *k2);
uint32_t hash_thomas_wang_str(const void* key);
uint32_t hash_identity(const void* num);
uint32_t hash_identity_attr_only(const void* pred);
uint32_t hash_thomas_wang_num(const void* num);
uint32_t hash_thomas_wang(const uint32_t num);
uint32_t hash_thomas_wang_rpredicate(const void* v);
uint32_t hash_thomas_wang_attr_value(const void* pred);
uint32_t hash_robert_jenkins_rpredicate(const void* pred);
uint32_t hash_thomas_wang_rpredicate_attr_only(const void* pred);
uint32_t hash_identity_rpredicate_attr_only(const void* pred);
uint32_t hash_identity_zpredicate_attr_only(const void* pred);
uint32_t hash_robert_jenkins(const uint32_t v);
uint8_t get_min_uint8_t(uint8_t n1, uint8_t n2);
uint8_t get_max_uint8_t(uint8_t n1, uint8_t n2);
uint32_t get_max_uint32_t(uint32_t n1, uint32_t n2);
PredScore get_max_predscore(PredScore n1, PredScore n2);
PredScore get_min_predscore(PredScore n1, PredScore n2);
uint32_t get_positive_predvalue_t(int n1);
int get_max_int(int n1, int n2);
int get_min_int(int n1, int n2);
uint32_t get_min_uint32_t(uint32_t n1, uint32_t n2);
PredValue get_min_predvalue_t(PredValue n1, PredValue n2);
PredValue get_max_predvalue_t(PredValue n1, PredValue n2);

inline struct boundary* createNewBoundary(PredValue minBoundary, PredValue maxBoundary);
inline void setBoundary(struct boundary *bound, PredValue minBoundary, PredValue maxBoundary);
inline void copyBoundary(struct boundary *oldBound, struct boundary *newBound);
inline int isBoundaryContained(struct boundary *mainBound, struct boundary *childBound);
inline int isBoundaryCrossHyperplane(struct boundary *objBoundary, PredValue hyperplane);
inline int isBoundaryLeftOfHyperplane(struct boundary *objBoundary, PredValue hyperplane);
inline int isBoundaryRightOfHyperplane(struct boundary *objBoundary, PredValue hyperplane);
inline int isBoundaryConflicted(struct boundary *leftBound, struct boundary *rightBound);
inline int isBoundaryOverlapped(struct boundary *firstBound, struct boundary *secondBound);
inline int isBoundaryConstraintPassed(struct boundary *newBound, struct boundary *leftBound, struct boundary *rightBound);
inline int isBoundaryNonEmptyOverlapped(struct boundary *firstBound, struct boundary *secondBound);
inline int isMustRightBoundary(struct boundary *rightBound, struct boundary *object);
inline int isMustLeftBoundary(struct boundary *leftBound, struct boundary *object);
inline PredValue getExpansionRate(struct boundary *mainBoundary, struct boundary *object);
inline PredValue getExpandedArea(struct boundary *mainBoundary, struct boundary *object);
inline int isBoundaryEqual(struct boundary *mainBound, struct boundary *childBound);
inline void updateBoundary(struct boundary *mainBound, struct boundary *addedBound);
inline PredValue getBoundaryArea(struct boundary *mainBound);
inline PredValue getRightOverlapBoundary(struct boundary *overlap, struct boundary *right);
inline PredValue getLeftOverlapBoundary(struct boundary *overlap, struct boundary *left);
inline int isBoundaryValid(struct boundary *mainBound);
inline int isContinuousOp(RPredicate *pred);

/* ALL THESE METHOD REQUIRE MEMORY CLEANUP AFTER GETTING THE STRING */
char* mapOptoString(int op);
char* mapMatchingAlgorithmtoString(int algorithm);
char* mapObjectiveFunctiontoString(int function);
char* mapMatchingTypetoString(int function);
char* mapClusterDirectoryTypetoString(int function);
char* mapTopkAlgorithmtoString(int topkType);
char* mapSplittingFunctiontoString(int function);
char* mapHashFunctoString(int hashFunc);



int compare_eqrank_rpredicate(const void *key1, const void *key2);
/* int compare_boundary_size_rpredicate(const void *key1, const void *key2); */


void print_object(Object *currSub);
void print_single_object(Object *currSub);
void print_single_object_with_score(Object *currSub, Object *currEvent);
void print_object_with_score(Object *currSub, Object *currEvent);

void print_object_stream(ObjectStream *currSub);
void print_single_object_stream(ObjectStream *currSub);

void resetSub(Object *sub, DataStructureParameter *dataStructureParameter);

#endif /* COMPARE_H_ */
