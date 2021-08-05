#include "stdafx.h"
#include "jfbuild/build.h"
#include "game.h"
#include "debug.h"


static uint64 frequency;		// The number of counts per second
static uint64 start;			// Counts when timer was initialized
static uint64 frequency_120;	// Counts per second translated into the game's frequency
static uint64 frequency_30;
static uint64 last_update_120 = 0;	// Last time we updated the clock
static uint64 last_update_30 = 0;


void timerInit()
{
//	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
//	QueryPerformanceCounter((LARGE_INTEGER *)&start);
	frequency_120 = frequency / 120;
	frequency_30  = frequency / 30;
}


void timerUpdate()
{
	// Get current ticks
	uint64 cur_ticks;
//	QueryPerformanceCounter((LARGE_INTEGER *)&cur_ticks);
	cur_ticks = 0;
	cur_ticks -= start;

	// Time to update?
	if ((cur_ticks - last_update_120) >= frequency_120)
	{
		last_update_120 = cur_ticks;
		gGameClock++;
		totalclock = gGameClock;
	}

	if ((cur_ticks - last_update_30) >= frequency_30)
	{
		last_update_30 = cur_ticks;
		gFrameClock += 4;
	}
}
