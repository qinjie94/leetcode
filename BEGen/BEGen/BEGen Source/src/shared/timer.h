/* timer.h
 *      Author: Mo Sadoghi
 *
*/

#ifndef TIMER_H_
#define TIMER_H_

typedef
struct rtree_timer{
	long		ElapsedTime;
	long		startElapsed, stopElapsed;
	float		UserTime, SystemTime;
}	TIMER;

#endif /* TIMER_H_ */
