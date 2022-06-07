#ifndef TIMER_H
#define TIMER_H

/*
 * this is a millisecond timer
 */

#include "PA9.h"

#define TIME_MGR_CAPACITY 16

struct tTimer
{
	u32 nPauseTick;
	u32 nRefTick;
	u8  bRunning;
};

struct tTimeMgr
{
	/* timer 0 as master timer */
	struct tTimer rBank[TIME_MGR_CAPACITY];
	u8            nCount;
};

extern void StartTime(u8 bNew);
extern void PauseTime();

extern u8   NewTimer(u8 bStarted);
extern void StartTimer(u8 nTimerId);
extern u32  Tick(u8 nTimerId);
extern void PauseTimer(u8 nTimerId);
extern void UnpauseTimer(u8 nTimerId);
extern void ResetTimer(u8);

#endif // TIMER_H
