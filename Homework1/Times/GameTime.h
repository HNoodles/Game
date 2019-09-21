#pragma once
#include "Timeline.h"

//using namespace std::chrono;

class GameTime : public Timeline
{
private:
	std::chrono::nanoseconds start_time;
	double step_size;
public:
	GameTime(double step_size);

	double getTime() override;
};

