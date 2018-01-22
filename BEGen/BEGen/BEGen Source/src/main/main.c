/*
 * main.c
 *
 *  Created on: Dec 12, 2009
 *      Author: Mo Sadoghi
 */
#include "../shared/constant.h"



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../workload/generateWorkLoad.h"
#include "../util/extractArguments.h"


int main(int argc, char **argv){
	ExperimentParameter *experimentParameter;
	clock_t start,end;
	long double dif;
	start = clock();

#ifndef NDEBUG
	puts("Debugging Begins...");
#endif
	experimentParameter = extract_argument(argc, argv);

	puts(experimentParameter->argument);

	if(experimentParameter->isRand){
		srand ( time(NULL) );
	}

	generate_workload(experimentParameter);

#ifndef NDEBUG
	puts("Debugging Ends...");
#endif

	end = clock();
	dif = (end - start) / (float) (CLOCKS_PER_SEC);
	printf ("Total time Execution time: %Lf.\n", dif );

return 0;

}
