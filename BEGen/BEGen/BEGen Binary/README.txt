--------------------------------------------------------------------------------
This is a framework for generating Boolean Expression, BEGen, 
workloads with different characteristics such as workload distribution, 
workload size, space dimensionality, average subscription and event size, 
dimension cardinality, predicate selectivity, dimension selectivity,  
subscription expressiveness, and event matching probability. BEGen is 
written in C. 

This framework is part of the work in "BE-Tree: An Index Structure to 
Efficiently Match Boolean Expressions over High-dimensional Discrete Space" 
(SIGMOD'11). For more inforamtion please see BE-Tree SIGMOD'11 paper.


For credits (either when referencing BE-Tree or utilizing BE-Gen) 
please use the following (bibtex):

@inproceedings{BETreeSadoghiSIGMOD11,
 author = {Sadoghi, Mohammad and Jacobsen, Hans-Arno},
 title = {BE-tree: an index structure to efficiently match boolean expressions over high-dimensional discrete space},
 booktitle = {Proceedings of the 2011 international conference on Management of data},
 series = {SIGMOD '11},
 year = {2011},
 isbn = {978-1-4503-0661-4},
 location = {Athens, Greece},
 pages = {637--648},
 numpages = {12},
 url = {http://doi.acm.org/10.1145/1989323.1989390},
 doi = {http://doi.acm.org/10.1145/1989323.1989390},
 acmid = {1989390},
 publisher = {ACM},
 address = {New York, NY, USA},
 keywords = {algorithm, boolean expressions, complex event processing, data structure, publish/subscribe},
} 
--------------------------------------------------------------------------------


--------------------------------------------------------------------------------
/* 
   In this package you will find binary (of C code) compiled on Ubuntu 11.04 with 
   kernel 2.6.38-11-generic using gcc (Ubuntu/Linaro 4.5.2-8ubuntu4) 4.5.2.
*/
--------------------------------------------------------------------------------
 




--------------------------------------------------------------------------------
/* Sample and required parameters for running BEGen code, please see bekow for detail explaination */
-------------------------------------------------------------------------------- 
-profileID 9
-qgramID 0

-rand 0

-subCount 10000
-subMinDim 6
-subMaxDim 8
-isInforceSubDim 1
-subOeq 1
-subOneq 0
-subOleq 1
-subOgeq 1
-subOleqgeq 1
-subOin 1
-subOnotin 0
-isInforceOeq 0
-subOeqRatio 0.3
-subUniform 1
-subZip 0
-subZipAlpha 3
-subUniformModel 0
-subWorkloadMatchingDegree 0.01
-subIsIncludeBaseTupe 1
-subMaxNumberInNotIn 10
-subMaxExtendedObjectSize 10
-subMaxExtendedObjectSizeUniform 0
-subMaxExtendedObjectSizeZip 0
-subHasMinExtendedObjectSize 0
-subMaxExtendedObjectSizeZipAlpha 0
-subIsRandomOrder 1
-subOutput sub.out
-subInput ../Data/dblp/extracted/author


-eventCount 1000
-eventMinDim 12
-eventMaxDim 16
-isInforceEventDim 1
-isEventHashEnabled 1
-eventOeq 1
-eventUniform 1
-eventZip 0
-eventZipAlpha 3
-eventUniformModel 0
-eventChosenFromSub 0
-eventOutput event.out
-eventInput ..
-matchingType 0

-systemDim 1000
-systemDimMeanScoreRatio 0
-systemDimVarianceScoreRatio 0
-systemDistinctValue 32
-systemMinDistinctValue 3
-systemMaxDistinctCluster 0
-systemMaxDistinctClusterSize 0
-systemDistinctValueUniform 1
-systemDistinctValueZip 0
-systemDistinctValueZipAlpha 0
-systemSubOptCount 7

-scoringType 0
-dataDistributionType 0
-------------------------------------------------------------------------------- 





--------------------------------------------------------------------------------
Manual (Explaining BEGen Input Parameters)
-------------------------------------------------------------------------------- 
/* 
Different type of workload
------------------------------------- 
/* Both subscriptions and events generated workload either Uniform or Zipf */
PROFILE_WORKLOAD_RANDOM_DATA 0  					

/* Random generated workload either Uniform or Zipf, where events are subset of subscriptions */
PROFILE_WORKLOAD_RANDOM_SUB_RANDOM_SUBSET_EVENT 1 	

/* Generated workload from real text data (qgram = 3) */
PROFILE_WORKLOAD_RAW_DATA 2							

/* Generated workload from real text data (qgram = 3), where the number of predicates per subscription and event are limited */
PROFILE_WORKLOAD_RAW_DATA_RESTRICTED_SIZE 3			

/* Generated workload from real text data (qgram = 3), where the number of predicates per subscription and event are reduced */
PROFILE_WORKLOAD_RAW_DATA_REDUCED_SIZE 4			

/* Generated workload from real text data (qgram = 3) while controlling the matching probability and subscription's predicates are equality only */
PROFILE_WORKLOAD_MATCHING_PROBABILITY_EQUALITY_RAW_DATA 5					

/* Generated workload from real text data (qgram = 3) while controlling the matching probability */
PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA 6

/* Generated workload from real text data (qgram = 3) while controlling the matching probability and the number of predicates per subscription and event are limited */
PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_RESTRICTED_SIZE 7

/* Generated workload from real text data (qgram = 3) while controlling the matching probability and the number of predicates per subscription and event are reduced */
PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RAW_DATA_REDUCED_SIZE 8

/* Generated workload from synthetic data while controlling the matching probability */
PROFILE_WORKLOAD_MATCHING_PROBABILITY_GENERIC_RANDOM_DATA 9
*/
-profileID 9        

-qgramID 0						/* Please use the default value of 0 */ 
-rand 0							/* C random number seed, for repeatable experiments please use zero as default value */

-subCount 10000						/* Number of subscriptions */
-subMinDim 6						/* Min number of subscription's predicate */
-subMaxDim 8						/* Max number of subscription's predicate */
-isInforceSubDim 1					/* Please use the default value of 1 */
-subOeq 1						/* If 1 it implies that subscription's predicate is allowed to have = operators */
-subOneq 0						/* If 1 it implies that subscription's predicate is allowed to have != operators */
-subOleq 1						/* If 1 it implies that subscription's predicate is allowed to have <= operators */
-subOgeq 1						/* If 1 it implies that subscription's predicate is allowed to have >= operators */
-subOleqgeq 1						/* If 1 it implies that subscription's predicate is allowed to have range operators */
-subOin 1						/* If 1 it implies that subscription's predicate is allowed to have equality set operators */
-subOnotin 0						/* If 1 it implies that subscription's predicate is allowed to have non-equality set operators */
-isInforceOeq 0						/* Please use the default value of 0 */
-subOeqRatio 0.3					/* Precentage of the number equality predicates with respect to other predicates within a subscription, The number 0.3 means, on average 30% of predicates are eqaulity */
-subUniform 1						/* If 1 it implies that the dimension of subscription are randomly chosen based on uniform distribution from a pool of available dimensions  */
-subZip 0						/* If 1 it implies that the dimension of subscription are randomly chosen based on Zipf distribution from a pool of available dimensions  */
-subZipAlpha 3						/* The Zipf parameter */
-subUniformModel 0					/* Please use default value of 0 */
-subWorkloadMatchingDegree 0.01				/* The degree of matching probablity, the number 0.01 means that on average, every event is matched with 1% of the workload */
-subIsIncludeBaseTupe 1					/* Please use default value of 1 */
-subMaxNumberInNotIn 10					/* The number of elements in non-equality set operator */
-subMaxExtendedObjectSize 10				/* The maximum of number of elements for range operators, the number 10 imples range of size 10 */
-subMaxExtendedObjectSizeUniform 0			/* Please use the default value of 0 */
-subMaxExtendedObjectSizeZip 0				/* Please use the default value of 0 */
-subHasMinExtendedObjectSize 0				/* Please use the default value of 0 */
-subMaxExtendedObjectSizeZipAlpha 0			/* Please use the default value of 0 */
-subIsRandomOrder 1					/* Please use the default value of 1 */
-subOutput sub.out					/* The location for storing subscription workload */
-subInput ../Data/dblp/extracted/author			/* The location of text data that is used to create the subscription and event */


-eventCount 1000					/* Number of events */
-eventMinDim 12						/* Min number of event's predicate */
-eventMaxDim 16						/* Max number of event's predicate */
-eventOeq 1						/* If 1 it implies that subscription's predicate is allowed to have = operators, for now, please always set it to 1 */
-eventUniform 1						/* For now, set it equal to the value of subUniform */
-eventZip 0						/* For now, set it equal to the value of subZipf */
-eventZipAlpha 3					/* For now, set it equal to the value of subZipAlpha */
-eventUniformModel 0					/* Please use default value of 0 */
-eventChosenFromSub 0					/* Please use default value of 0 */
-eventOutput event.out					/* The location for storing event workload */
-eventInput ..						/* For now, set it equal to the value of subInput */

-systemDim 1000						/* The number of dimensions */
-systemDimMeanScoreRatio 0				/* Please use default value of 0 */
-systemDimVarianceScoreRatio 0				/* Please use default value of 0 */
-systemDistinctValue 32					/* Max cardinality of each domain */
-systemMinDistinctValue 3				/* Min cardinality of each domain */
-systemMaxDistinctCluster 0				/* Please use default value of 0 */
-systemMaxDistinctClusterSize 0				/* Please use default value of 0 */
-systemDistinctValueUniform 1				/* Please use default value of 1 */
-systemDistinctValueZip 0				/* Please use default value of 0 */
-systemDistinctValueZipAlpha 0				/* Please use default value of 0 */
-systemSubOptCount 7					/* Number of operator in the predicate language, please use the default value of 7 */

-scoringType 0						/* Please use default value of 0 */
-dataDistributionType 0					/* Please use default value of 0 */
-------------------------------------------------------------------------------- 





--------------------------------------------------------------------------------
Manual (Explaining BEGen Workload Output File Format) -- sub.out or event.out ---
--------------------------------------------------------------------------------
/* The number of dimensions */ 
/* Cardinaity of each dimension starting from 1 ... systemDim, where dimension 0 is not actually used */
$systemDim 100$						

/* Max cardinality of each domain */
/* Cardinaity of each dimension starting from 1 ... systemDistinctValue, where domain value 0 is not actually used */
$systemDistinctValue 15$	
		

/* The format follows: dimension_cardinality score; dimension_cardinality score; dimension_cardinality score; ... */
/* where the first pair (dimension_cardinality score) is for dimension 1, the second pair for dimension 2, etc. */
/* For now please ignore the score value */  		
0 0.000000; 8 0.142857; 6 0.250000; 3 0.250000; 8 0.111111; 8 0.125000; 3 0.142857; 3 0.250000; 15 0.142857; 3 1.000000; 7 0.111111; 11 0.111111; 13 0.200000; 12 0.125000; 8 0.166667; 9 0.166667; 14 0.142857; 5 0.166667; 4 0.166667; 3 0.166667; 6 0.111111; 7 0.142857; 8 0.142857; 14 0.250000; 13 0.500000; 3 0.100000; 3 0.142857; 10 0.142857; 9 0.333333; 9 0.100000; 12 0.125000; 10 0.250000; 3 0.500000; 3 0.142857; 6 0.200000; 6 0.200000; 13 0.166667; 9 0.250000; 12 0.250000; 3 0.166667; 12 0.166667; 3 0.166667; 3 0.500000; 8 0.333333; 14 0.142857; 14 0.076923; 3 0.200000; 13 0.166667; 6 0.200000; 5 0.200000; 12 0.166667; 13 0.142857; 15 0.142857; 9 0.142857; 3 0.083333; 12 0.200000; 8 0.111111; 3 0.200000; 8 0.166667; 3 0.250000; 15 0.100000; 10 0.111111; 5 0.200000; 3 0.166667; 8 0.125000; 6 0.111111; 15 0.142857; 14 0.250000; 10 0.100000; 14 0.111111; 11 0.166667; 5 0.200000; 3 0.166667; 3 0.142857; 4 0.333333; 14 0.066667; 3 0.142857; 6 0.200000; 15 0.250000; 15 0.166667; 13 0.166667; 10 0.333333; 6 0.111111; 5 0.111111; 4 0.250000; 7 0.142857; 3 0.250000; 3 0.333333; 3 0.111111; 3 0.142857; 13 1.000000; 13 0.125000; 11 0.166667; 3 0.100000; 11 0.200000; 8 0.111111; 9 0.200000; 3 0.200000; 10 0.250000; 4 0.333333; 9 0.166667;

/* Number of subscriptions/events */
$ObjectCount 198$

/* Max number of subscription's/event's predicate */
$ObjectMaxDim 8$

/* Min number of subscription's/event's predicate */
$ObjectMinDim 1$

/* Please ignore */
$EqObjectCount$ 6$

/* Average number of equality predicate per subscriptions/events */
$AvgEqOpCount 3.75$

/* Average number of equality predicate per subscriptions/events */
$AvgNonEqOpCount 1.33$

/* Average number of predicate per subscriptions/events */
$AvgObjectDim 5.08$


A sample line that describes a single subscription/event consists of three parts
[61, 5, 5]	1 0 0.000000 |1 0 0.000000 |1 0 0.000000 |1 0 0.000000 |1 0 0.000000 |127822, 1, 13, 1, 8 8, 8 560386, 1, 57, 1, 2 2, 2 973288, 1, 99, 1, 1 1, 1 68836, 1, 7, 1, 3 3, 3 589879, 1, 60, 1, 14 14, 14

Part 1) High-level subscription/event information
[61, 5, 5]

which can parsed as follows

[(int) subscription/event id, (int) number of predicates, (int) number of equality predicates]


Part 2) Please ignore

1 0 0.000000 |1 0 0.000000 |1 0 0.000000 |1 0 0.000000 |1 0 0.000000

which can parsed as follows
 
"(int) please ignore"  "(int) please ignore"  "(float) please ignore" | 

repeated X times, where X is equal to the number predicates per subscription/event


Part 3) Predicate information

127822, 1, 13, 1, 8 8, 8 560386, 1, 57, 1, 2 2, 2 973288, 1, 99, 1, 1 1, 1 68836, 1, 7, 1, 3 3, 3 589879, 1, 60, 1, 14 14, 14


which can parsed as follows (this set of values are repeated for each predicate in the subscription/event)

(int) predicate id, (int) number of values, (int) dimension id/dimension name, (int) operator id, (int int) the range of value, (int ... int) each individual value

The number of value depends on type of operator, for instance

i) = has one value x = 5
ii) > has one value x > 5
iii) [] has two values x has a range [3, 9]

Operator id are as follows:

#define OPT_EQ 1
#define OPT_NEQ 2
#define OPT_LEQ 3
#define OPT_GEQ 4
#define OPT_LGEQ 5
#define OPT_IN 6
#define OPT_NOTIN 7

Examples of range of values (please refer predicate transformation description in SIGMOD'11 paper)

i) = has one value x = 5 has range [5, 5]
ii) > has one value x > 5 [5, infinity]
iii) [] has two values x has a range [3, 9] which clearly has range [3, 9]
------------------------------------------------------------------------------------------



------------------------------------------------------------------------------------------
BEGen 1.0 is release on October 12, 2011 by Mohammad Sadoghi
------------------------------------------------------------------------------------------
Middleware Systems Research Group (http://msrg.org)
Department of Computer Science
University of Toronto

------------------------------------------------------------------------------------------
Update Log
------------------------------------------------------------------------------------------
-The README file was update on Nov. 27, 2011.
------------------------------------------------------------------------------------------


------------------------------------------------------------------------------------------
Copyright 2011 Mohammad Sadoghi. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Mohammad Sadoghi ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Mohammad Sadoghi OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Mohammad Sadoghi.
------------------------------------------------------------------------------------------
