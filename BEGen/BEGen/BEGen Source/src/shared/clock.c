/*
 *    clock.c
 *    Author: Mo Sadoghi
 *
*/

#include "clock.h"

#ifdef	VMS
tbuffer_t start, stop, net;
#else
struct	tms start, stop, net;
#endif

void ResetClock(TIMER *timerp){
	timerp->UserTime = timerp->SystemTime = 0;
	timerp->ElapsedTime = 0;

#ifdef	VMS
	net.proc_user_time = net.proc_system_time = 0;
#else
	net.tms_utime = net.tms_stime = net.tms_cutime = net.tms_cstime = 0;
#endif
}

void StartClock(TIMER *timerp){
	long time();

	times(&start);
	timerp->startElapsed = time(0);
}

void StopClock(TIMER *timerp){
	long time();

	times(&stop);
	timerp->stopElapsed = time(0);

#ifdef	VMS
	net.proc_user_time +=  stop.proc_user_time -  start.proc_user_time;
	net.proc_system_time +=  stop.proc_system_time -  start.proc_system_time;

	timerp->UserTime = (float)net.proc_user_time / 60;
	timerp->SystemTime = (float)net.proc_system_time / 60;
#else
	net.tms_utime +=  stop.tms_utime -  start.tms_utime;
	net.tms_stime +=  stop.tms_stime -  start.tms_stime;
	net.tms_cutime += stop.tms_cutime - start.tms_cutime;
	net.tms_cstime += stop.tms_cstime - start.tms_cstime;

	timerp->UserTime = (float)net.tms_utime / 60;
	timerp->SystemTime = (float)net.tms_stime / 60;
#endif

	timerp->ElapsedTime += timerp->stopElapsed - timerp->startElapsed;
}
