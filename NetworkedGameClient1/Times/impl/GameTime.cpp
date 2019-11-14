#include "../GameTime.h"

GameTime::GameTime(double step_size) : Timeline(step_size), start_time(getNowRealTime())
{
}

//GameTime::GameTime(double step_size, double time) : Timeline(step_size)
//{
//	start_time = getNowRealTime() - duration<int>((int)(time * step_size));
//}

void GameTime::resetStepSize(double new_step_size)
{// goal is to reset start time, realizing resetting step size without changing current time tics
	new_step_size = new_step_size * 1e9;

	// old step size * old pt = new step size * new pt
	double newPausedTics = (step_size / 1e9) * pausedTics / (new_step_size / 1e9);

	// getTime should remain unchanged, thus we have:
	// (real time - old start time) / old step size - old paused tics
	// = (real time - new start time) / new step size - new paused ticsw
	// then, we can compute new start time now
	nanoseconds real = getNowRealTime();
	double temp = (real - start_time).count() / step_size - pausedTics + newPausedTics;
	nanoseconds new_start_time = 
		duration<long long, std::nano>(real.count() - (long long)(temp * new_step_size));

	// reset all fields
	step_size = new_step_size;
	start_time = new_start_time;
	pausedTics = newPausedTics;
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

