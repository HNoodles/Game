#pragma once
#include "Timeline.h"

class LocalTime : public Timeline
{
private:
	Timeline& anchor;
	double step_size;
public:
	LocalTime(double step_size, Timeline& anchor);

	void setPaused(bool p) override;

	void setStepSize(double step_size) override;

	double getTime() override;
};

