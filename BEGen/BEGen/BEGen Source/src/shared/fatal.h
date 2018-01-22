/*
 * fatal.h
 *
 *  Created on: Apr 22, 2009
 *      Author: Mo Sadoghi
 */
#include <stdio.h>
#include <stdlib.h>

#define fatal_error( Str )   error( Str ), exit( 1 )
#define error( Str )         fprintf( stderr, "\nERROR: %s\n", Str )

