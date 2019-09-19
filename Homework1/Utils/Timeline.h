#pragma once
#include <chrono>

using namespace std::chrono;

class Timeline
{
public:
	std::chrono::milliseconds getNow()
	{
		return duration_cast<std::chrono::milliseconds>(system_clock::now() - system_clock::from_time_t(0));
	}
	virtual int getTime() = 0;
};