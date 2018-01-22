/*
 * generateWorkLoad.h
 *
 *  Created on: Nov 6, 2009
 *      Author: Mo Sadoghi
 */

#ifndef GENERATEWORKLOAD_H_
#define GENERATEWORKLOAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../shared/generic.h"
#include "../shared/commontype.h"
#include "../shared/constant.h"
#include "../shared/fatal.h"
#include "../randgen/randgen.h"
#include "../workload/workload.h"

void generate_workload(ExperimentParameter *experimentParameter);

void setSubWorkloadParameter(WorkloadParameter *workloadParameter, const ExperimentParameter* experiment);
void setEventWorkloadParameter(WorkloadParameter *workloadParameter, const ExperimentParameter* experiment);

#endif /* GENERATEWORKLOAD_H_ */
