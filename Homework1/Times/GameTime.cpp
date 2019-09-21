#include "GameTime.h"

GameTime::GameTime(double step_size) : step_size(step_size), start_time(getNow())
{
}

double GameTime::getTime()
{
	nanoseconds elaspsed = getNow() - start_time;
	return elaspsed.count() / step_size;
}
