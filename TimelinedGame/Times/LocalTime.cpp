#include "LocalTime.h"

LocalTime::LocalTime(double step_size, Timeline& anchor) : step_size(step_size), anchor(anchor)
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

void LocalTime::setStepSize(double step_size)
{
	this->step_size = step_size * 1e9;
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
