/*
 * generictype.h
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 */

#ifndef _GenericTypes_h
#define _GenericTypes_h

#include "stdint.h"

typedef void* ElementType;
typedef uint32_t PredValue;
typedef float PredScore;
typedef struct key_value_pair KeyValuePair;

struct key_value_pair{
	void *key;
	void *value;
};

#endif



