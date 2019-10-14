#pragma once
#include "Movable.h"

class MovingPlatform : public RectangleShape, public Movable, 
{
private:
	//Transform& transform;
	bool headingPositive;
	float leftBound, rightBound;
public:
	MovingPlatform(Vector2f size, Vector2f v, float lb, float range, Timeline& timeline);

	void update(double elapsed) override;

	bool getHeadingPositive() const 
	{
		return headingPositive;
	}

	void setHeadingPositive(bool h)
	{
		headingPositive = h;
	}
};

