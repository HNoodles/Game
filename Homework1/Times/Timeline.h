#pragma once
#include <chrono>

using namespace std::chrono;

class Timeline
{
protected:
	bool paused = false;
	double pausedTics = 0;
public:
	std::chrono::nanoseconds getNow()
	{
		time_point<system_clock> zero;
		return duration_cast<std::chrono::nanoseconds>(system_clock::now() - zero);
	}
	virtual double getTime() = 0;
};