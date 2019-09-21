#pragma once
#include "Timeline.h"

class LocalTime : public Timeline
{
private:
	Timeline& anchor;
	double step_size;
public:
	LocalTime(double step_size, Timeline& anchor);

	double getTime() override;
};

