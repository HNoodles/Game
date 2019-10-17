#include "GameTime.h"

GameTime::GameTime(double step_size) : step_size(step_size * 1e9), start_time(getNowRealTime())
{
}

GameTime::GameTime(double step_size, double time) : step_size(step_size * 1e9)
{
	start_time = getNowRealTime() - duration<int>((int)(time * step_size));
}

void GameTime::setPaused(bool p) 
{
	if (p)
	{
		lastPausedPoint = getTime();
	}
	else
	{
		nanoseconds elaspsed = getNowRealTime() - start_time;
		double elapsedTics = elaspsed.count() / step_size;
		pausedTics = elapsedTics - lastPausedPoint;
	}

	paused = p;
}

void GameTime::setStepSize(double step_size)
{
	this->step_size = step_size * 1e9;
}

double GameTime::getTime()
{
	if (paused)
	{
		return lastPausedPoint;
	}
	else
	{
		nanoseconds elaspsed = getNowRealTime() - start_time;
		return elaspsed.count() / step_size - pausedTics;
	}
}

