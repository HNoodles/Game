#pragma once
#include <chrono>

using namespace std::chrono;

class Timeline
{
protected:
	bool paused = false;
	double pausedTics = 0;
	double lastPausedPoint = 0;
public:
	std::chrono::nanoseconds getNowRealTime()
	{
		return duration_cast<std::chrono::nanoseconds>(system_clock::now().time_since_epoch());
	}

	bool getPaused() 
	{
		return paused;
	}

	virtual void setPaused(bool p) = 0;

	virtual void setStepSize(double step_size) = 0;

	virtual double getTime() = 0;
};