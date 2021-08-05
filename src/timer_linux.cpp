#include "stdafx.h"
#include "globals.h"
#include "debug.h"
#include "error.h"

#include "jfbuild/compat.h"


struct timespec_ex : public timespec
{
	void reset();

	timespec_ex &operator  = (const timespec_ex &rhs);
	timespec_ex &operator -= (const timespec_ex &rhs);
};


void timespec_ex::reset()
{
	tv_sec  = 0;
	tv_nsec = 0;
}


timespec_ex &timespec_ex::operator = (const timespec_ex &rhs)
{
	tv_sec  = rhs.tv_sec;
	tv_nsec = rhs.tv_nsec;

	return *this;
}


timespec_ex &timespec_ex::operator -= (const timespec_ex &rhs)
{
	// Do nanoseconds
	long left = rhs.tv_nsec;
	if (left >= tv_nsec)
	{
		left -= tv_nsec;
		if (left > 0)
		{
			if (tv_sec > 0) tv_sec--;
			tv_nsec = 1000000000 - left;
		}
		else
		{
			tv_nsec = 0;
		}
	}
	else
	{
		tv_nsec -= left;
	}

	// Do seconds
	assert(tv_sec >= rhs.tv_sec);
	tv_sec -= rhs.tv_sec;

	return *this;
}


static timespec_ex ts_freq;		// Frequency of monotonic timer
static timespec_ex ts_start;	// The start of the timer (for making the timing range smaller)
static timespec_ex ts_last_sec;	// Timestamp of last second (for FPS calculation)


static void FormatTime(char *buffer, unsigned size, const timespec_ex *ts)
{
	Bsnprintf(buffer, size, "%ld.%09ld", ts->tv_sec, ts->tv_nsec);
}


void timerInit()
{
	dprintf("Initializing timer...\n");

	if (clock_getres(CLOCK_MONOTONIC, &ts_freq)   == -1) ThrowError("Failed getting timer frequency");
	if (clock_gettime(CLOCK_MONOTONIC, &ts_start) == -1) ThrowError("Failed getting start time");

	char buffer[32];
	FormatTime(buffer, sizeof(buffer) - 1, &ts_freq);
	dprintf("Frequency : %s\n", buffer);
	FormatTime(buffer, sizeof(buffer) - 1, &ts_start);
	dprintf("Start time: %s\n", buffer);

	ts_last_sec.reset();
}


void timerUpdate()
{
	timespec_ex ts_current;
//	char current[32], start[32], newtime[32];

	// Get current time, and subtract start time from it
	clock_gettime(CLOCK_MONOTONIC, &ts_current);
	ts_current -= ts_start;

	// Has it been a second since last time?
	timespec_ex ts_delta_seconds;
	ts_delta_seconds = ts_current;
	ts_delta_seconds -= ts_last_sec;
	if (ts_delta_seconds.tv_sec >= 1)
	{
		ts_last_sec = ts_current;
		gFrameRate = gFrames;
		gFrames = 0;
	}
}
