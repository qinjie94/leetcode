/*
 * extractArgument.c
 *
 *  Created on: Dec 12, 2009
 *      Author: Mo Sadoghi
 */

#include "extractArguments.h"

ExperimentParameter* extract_argument(int argc, char **argv) {
	ExperimentParameter *experimentParameter;
	char flag[500], value[500];

	experimentParameter = malloc(sizeof(ExperimentParameter));


	/* Testing Code Ends */
	strcpy(flag, EMPTY_STRING);
	strcpy(value, EMPTY_STRING);

	ini_experiment_parameter(experimentParameter);

	if(argc--){
		strcat(experimentParameter->argument, *argv++);

		while(argc-- && argc--){
			strcpy(flag, *argv++);
			strcpy(value, *argv++);

			strcat(experimentParameter->argument, " ");
			strcat(experimentParameter->argument, flag);
			strcat(experimentParameter->argument, " ");
			strcat(experimentParameter->argument, value);

			if(strcmp(flag, "-profileID") == 0) {
				experimentParameter->profileID = atoi(value);

			}else if(strcmp(flag, "-qgramID") == 0) {
					experimentParameter->qgramID = atoi(value);

			}else if(strcmp(flag, "-rand") == 0) {
				experimentParameter->isRand = atoi(value);

			}else if(strcmp(flag, "-subCount") == 0) {
				experimentParameter->subCount = atoi(value);

			}else if(strcmp(flag, "-subMaxDim") == 0) {
				experimentParameter->subMaxDim = atoi(value);

			}else if(strcmp(flag, "-subMinDim") == 0) {
				experimentParameter->subMinDim = atoi(value);

			}else if(strcmp(flag, "-isInforceSubDim") == 0) {
				experimentParameter->isInforceSubDim = atoi(value);

			}else if(strcmp(flag, "-subOeq") == 0) {
				experimentParameter->subOeq = atoi(value);

			}else if(strcmp(flag, "-subOneq") == 0) {
				experimentParameter->subOneq = atoi(value);

			}else if(strcmp(flag, "-subOleq") == 0) {
				experimentParameter->subOleq = atoi(value);
			}

			else if(strcmp(flag, "-subOgeq") == 0) {
				experimentParameter->subOgeq = atoi(value);
			}

			else if(strcmp(flag, "-subOleqgeq") == 0) {
				experimentParameter->subOlgeq = atoi(value);
			}

			else if(strcmp(flag, "-subOin") == 0) {
				experimentParameter->subOin = atoi(value);
			}

			else if(strcmp(flag, "-subOnotin") == 0) {
				experimentParameter->subOnotin = atoi(value);

			}else if(strcmp(flag, "-isInforceOeq") == 0) {
				experimentParameter->isInforceOeq = atoi(value);

			}else if(strcmp(flag, "-subOeqRatio") == 0) {
				experimentParameter->subOeqRatio = atof(value);

			}else if(strcmp(flag, "-subMaxExtendedObjectSize") == 0) {
				experimentParameter->subMaxExtendedObjectSize = atoi(value);

			}else if(strcmp(flag, "-subMaxExtendedObjectSizeUniform") == 0) {
				experimentParameter->subMaxExtendedObjectSizeUniform = atoi(value);

			}else if(strcmp(flag, "-subMaxExtendedObjectSizeZip") == 0) {
				experimentParameter->subMaxExtendedObjectSizeZip = atoi(value);

			}else if(strcmp(flag, "-subMaxExtendedObjectSizeZipAlpha") == 0) {
				experimentParameter->subMaxExtendedObjectSizeZipAlpha = atoi(value);

			}else if(strcmp(flag, "-subHasMinExtendedObjectSize") == 0) {
				experimentParameter->subHasMinExtendedObjectSize = atoi(value);
			}

			else if(strcmp(flag, "-subMaxNumberInNotIn") == 0) {
				experimentParameter->subMaxNumberInNotIn = atoi(value);
			}

			else if(strcmp(flag, "-subUniform") == 0) {
				experimentParameter->subUniform = atoi(value);
			}

			else if(strcmp(flag, "-subZip") == 0) {
				experimentParameter->subZip = atoi(value);
			}

			else if(strcmp(flag, "-subZipAlpha") == 0) {
				experimentParameter->subZipAlpha = atoi(value);
			}

			else if(strcmp(flag, "-subUniformModel") == 0) {
				experimentParameter->subUniformModel = atoi(value);
			}

			else if(strcmp(flag, "-subWorkloadMatchingDegree") == 0) {
				experimentParameter->subWorkloadMatchingDegree = atof(value);
			}

			else if(strcmp(flag, "-subIsIncludeBaseTupe") == 0) {
				experimentParameter->subIsIncludeBaseTupe = atoi(value);
			}

			else if(strcmp(flag, "-subIsRandomOrder") == 0) {
				experimentParameter->subIsRandomOrder = atoi(value);
			}


			else if(strcmp(flag, "-subOutput") == 0) {
				strcpy(experimentParameter->subOutput, value);

			}else if(strcmp(flag, "-subInput") == 0) {
				strcpy(experimentParameter->subInput, value);

			}else if(strcmp(flag, "-eventCount") == 0) {
				experimentParameter->eventCount = atoi(value);

			}else if(strcmp(flag, "-eventMaxDim") == 0) {
				experimentParameter->eventMaxDim = atoi(value);

			}else if(strcmp(flag, "-eventMinDim") == 0) {
				experimentParameter->eventMinDim = atoi(value);

			}else if(strcmp(flag, "-isInforceEventDim") == 0) {
				experimentParameter->isInforceEventDim = atoi(value);

			}else if(strcmp(flag, "-isEventHashEnabled") == 0) {
				experimentParameter->isEventHashEnabled = atoi(value);

			}else if(strcmp(flag, "-eventOeq") == 0) {
				experimentParameter->eventOeq = atoi(value);
			}

			else if(strcmp(flag, "-eventUniform") == 0) {
				experimentParameter->eventUniform = atoi(value);
			}

			else if(strcmp(flag, "-eventZip") == 0) {
				experimentParameter->eventZip = atoi(value);
			}

			else if(strcmp(flag, "-eventZipAlpha") == 0) {
				experimentParameter->eventZipAlpha = atoi(value);
			}


			else if(strcmp(flag, "-eventUniformModel") == 0) {
				experimentParameter->eventUniformModel = atoi(value);
			}

			else if(strcmp(flag, "-eventChosenFromSub") == 0) {
				experimentParameter->eventChosenFromSub = atoi(value);
			}

			else if(strcmp(flag, "-eventOutput") == 0) {
				strcpy(experimentParameter->eventOutput, value);

			}else if(strcmp(flag, "-eventInput") == 0) {
				strcpy(experimentParameter->eventInput, value);


			}else if(strcmp(flag, "-matchingType") == 0) {
				experimentParameter->matchingType = atoi(value);

			}else if(strcmp(flag, "-systemDimMeanScoreRatio") == 0) {
				experimentParameter->systemDimMeanScoreRatio = atof(value);

			}else if(strcmp(flag, "-systemDimVarianceScoreRatio") == 0) {
				experimentParameter->systemDimVarianceScoreRatio = atof(value);

			}else if(strcmp(flag, "-systemDim") == 0) {
				experimentParameter->systemDim = atoi(value);
			}

			else if(strcmp(flag, "-systemDistinctValue") == 0) {
				experimentParameter->systemDistinctValue = atol(value);
			}

			else if(strcmp(flag, "-systemMinDistinctValue") == 0) {
				experimentParameter->systemMinDistinctValue = atol(value);
			}

			else if(strcmp(flag, "-systemMaxDistinctCluster") == 0) {
				experimentParameter->systemMaxDistinctCluster = atoi(value);
			}

			else if(strcmp(flag, "-systemMaxDistinctClusterSize") == 0) {
				experimentParameter->systemMaxDistinctClusterSize = atoi(value);
			}


			else if(strcmp(flag, "-systemDistinctValueUniform") == 0) {
				experimentParameter->systemDistinctValueUniform = atoi(value);
			}

			else if(strcmp(flag, "-systemDistinctValueZip") == 0) {
				experimentParameter->systemDistinctValueZip = atoi(value);
			}


			else if(strcmp(flag, "-systemDistinctValueZipAlpha") == 0) {
				experimentParameter->systemDistinctValueZipAlpha = atoi(value);
			}

			else if(strcmp(flag, "-systemSubOptCount") == 0) {
				experimentParameter->systemSubOptCount = atoi(value);
			}

			else if(strcmp(flag, "-scoringType") == 0) {
				experimentParameter->scoringType = atoi(value);
			}

			else if(strcmp(flag, "-dataDistributionType") == 0) {
				experimentParameter->dataDistributionType = atoi(value);
			}

			else if(strcmp(flag, "-hashFunc") == 0) {
				experimentParameter->hashFunc = atoi(value);
			}

			else{
				strcat(flag, " ");
				strcat(flag, value);
				strcpy(value, flag);
				strcpy(flag, "Incorrect Parameters! --> ");
				strcat(flag, value);
				fatal_error(flag);
			}
		}
	}

	return experimentParameter;
}







