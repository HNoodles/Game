#pragma once
#include "Timeline.h"

//using namespace std::chrono;

class GameTime : public Timeline
{
private:
	std::chrono::milliseconds start_time;
	int step_size;
public:
	GameTime(int step_size);

	int getTime() override;
};

