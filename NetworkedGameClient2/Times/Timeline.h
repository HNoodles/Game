#pragma once
#include <chrono>

using namespace std::chrono;

class Timeline
{
protected:
	double step_size;
	bool paused = false;
	double pausedTics = 0;
	double lastPausedPoint = 0;
public:
	Timeline(double step_size)
	{
		setStepSize(step_size);
	}

	std::chrono::nanoseconds getNowRealTime()
	{
		return duration_cast<std::chrono::nanoseconds>(system_clock::now().time_since_epoch());
	}

	bool getPaused() 
	{
		return paused;
	}

	void setStepSize(double step_size)
	{
		this->step_size = step_size * 1e9;
	}

	double getStepSize()
	{
		return step_size / 1e9;
	}

	virtual void setPaused(bool p) = 0;

	virtual double getTime() = 0;
};