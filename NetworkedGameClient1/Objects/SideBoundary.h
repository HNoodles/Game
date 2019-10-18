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
	Vector2f windowSize;
	float padding;
public:
	SideBoundary(::Direction direction, Vector2f windowSize, float padding); 

	Vector2f getOffset() const;

	void updatePos(::Direction direction);
};

