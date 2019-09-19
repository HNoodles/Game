#pragma once
#include "Platform.h"
#include "Movable.h"

class MovingPlatform : public Platform, public Movable
{
private:
	//Transform& transform;
	bool headingPositive;
	float leftBound, rightBound;
public:
	MovingPlatform(Vector2f size, Vector2f v, float lb, float range);

	void around(RenderTarget& target) override;

	bool getHeadingPositive() const 
	{
		return headingPositive;
	}
};

