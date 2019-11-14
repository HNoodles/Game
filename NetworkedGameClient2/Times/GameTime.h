#pragma once
#include "Timeline.h"

class GameTime : public Timeline
{
private:
	std::chrono::nanoseconds start_time;
public:
	GameTime(double step_size);

	//GameTime(double step_size, double time);

	void setPaused(bool p) override;

	double getTime() override;
};

