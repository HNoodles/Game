#include "../LocalTime.h"

LocalTime::LocalTime(double step_size, Timeline& anchor) : Timeline(step_size), anchor(anchor)
{
}

void LocalTime::setPaused(bool p)
{
	if (p)
	{
		lastPausedPoint = getTime();
	}
	else
	{
		pausedTics = anchor.getTime() - lastPausedPoint;
	}

	paused = p;
}

double LocalTime::getTime()
{// get elapsed time relative to anchored time
	if (paused)
	{
		return lastPausedPoint;
	}
	else
	{
		nanoseconds now = getNowRealTime();
		return now.count() / step_size - anchor.getTime() - pausedTics;
	}
}
