#include "LocalTime.h"

LocalTime::LocalTime(double step_size, Timeline& anchor) : step_size(step_size), anchor(anchor)
{
}

double LocalTime::getTime()
{// get elapsed time relative to anchored time
	nanoseconds now = getNow();
	return now.count() / step_size - anchor.getTime();
}
