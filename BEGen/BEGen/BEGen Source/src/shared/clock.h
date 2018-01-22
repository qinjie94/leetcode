/*
 * clock.h
 *
 *  Created on: Dec 25, 2009
 *      Author: Mo Sadoghi
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#ifdef	VMS
#include	<time.h>
#else
#include	<sys/types.h>
#include	<sys/times.h>
#endif

#include "timer.h"


void ResetClock(TIMER *timerp);
void StartClock(TIMER *timerp);
void StopClock(TIMER *timerp);


#endif /* CLOCK_H_ */
