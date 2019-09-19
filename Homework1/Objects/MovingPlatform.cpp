#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(Vector2f size, Vector2f v, float lb, float range)
	: RectangleShape(size), headingPositive(true), leftBound(lb), rightBound(lb + size.x + range)
{
	velocity = v;
}

void MovingPlatform::around(RenderTarget& target)
{
	Vector2u target_size = target.getSize();
	Vector2f pos = this->getPosition();
	Vector2f size = this->getSize();

	if ((headingPositive && pos.x + size.x <= rightBound)
		|| (!headingPositive && pos.x <= leftBound)) 
	{// heading right, not reaching bound
	// heading left, reached bound
		headingPositive = true;
		this->move(velocity);
	}
	else if ((headingPositive && pos.x + size.x > rightBound) 
		|| (!headingPositive && pos.x > leftBound))
	{// heading right, reached bound
	// heading left, not reaching bound
		headingPositive = false;
		this->move(-velocity);
	}
}
