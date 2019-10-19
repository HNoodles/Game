#pragma once
#include "GameObject.h"
#include "../Components/Collidable.h"

enum class Direction {
	LEFT, RIGHT, UP, BOTTOM
};

class SideBoundary : public GameObject
{
private:
	::Direction direction;
	Vector2u windowSize;
	float padding;
	Vector2f offset;
public:
	SideBoundary(::Direction direction, Vector2u windowSize, float padding); 

	Vector2f getOffset() const
	{
		return offset;
	}

	::Direction getDirection() const
	{
		return direction;
	}

	void updatePos(Vector2f offset);
};

