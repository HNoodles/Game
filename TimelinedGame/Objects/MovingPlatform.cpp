#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(Vector2f size, Vector2f v, float lb, float range, Timeline& timeline)
	: RectangleShape(size), Movable(v, timeline), headingPositive(true), leftBound(lb), rightBound(lb + size.x + range)
{
}

void MovingPlatform::update(RenderTarget& target, double elapsed)
{
	Vector2u target_size = target.getSize();
	Vector2f pos = this->getPosition();
	Vector2f size = this->getSize();

	// calculate displacement
	Vector2f s = Vector2f(velocity.x * elapsed, velocity.y * elapsed);

	// move object
	if ((headingPositive && pos.x + size.x <= rightBound)
		|| (!headingPositive && pos.x <= leftBound)) 
	{// heading right, not reaching bound
	// heading left, reached bound
		headingPositive = true;
		this->move(s);
	}
	else if ((headingPositive && pos.x + size.x > rightBound) 
		|| (!headingPositive && pos.x > leftBound))
	{// heading right, reached bound
	// heading left, not reaching bound
		headingPositive = false;
		this->move(-s);
	}
}
