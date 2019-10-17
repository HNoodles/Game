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

	GameTime(double step_size, double time);

	void setPaused(bool p) override;

	void setStepSize(double step_size) override;

	double getTime() override;
};

