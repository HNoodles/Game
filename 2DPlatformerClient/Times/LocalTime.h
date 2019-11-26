#pragma once
#include "Timeline.h"

class LocalTime : public Timeline
{
private:
	Timeline& anchor;
public:
	LocalTime(double step_size, Timeline& anchor);

	void setPaused(bool p) override;

	double getTime() override;
};

