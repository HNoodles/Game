#pragma once
#include "GameObject.h"
#include "../Components/Collidable.h"

class MovingPlatform : public GameObject
{
private:
	bool headingPositive;
	float negBound, posBound;
public:
	MovingPlatform(
		::Shape shape, ::Color color, Vector2f size, Vector2f pos,
		Vector2f velocity, Timeline& timeline, ::Move move, float negBound = 0, float range = 0
	);

	bool getHeadingPositive() const
	{
		return headingPositive;
	}

	void setHeandingPositive(bool headingPositive)
	{
		this->headingPositive = headingPositive;
	}

	float getNegBound() const
	{
		return negBound;
	}

	void setNegBound(float negBound)
	{
		this->negBound = negBound;
	}

	float getPosBound() const
	{
		return posBound;
	}

	void setPosBound(float posBound)
	{
		this->posBound = posBound;
	}
};

