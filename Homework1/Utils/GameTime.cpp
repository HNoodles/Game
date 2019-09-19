#include "GameTime.h"

GameTime::GameTime(int step_size)
{
	this->step_size = step_size;
	start_time = getNow();
}

int GameTime::getTime()
{
	milliseconds elaspsed = getNow() - start_time;
	return (int)elaspsed.count() / step_size;
}
