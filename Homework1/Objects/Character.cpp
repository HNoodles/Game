#include "Character.h"

Vector2f gravity(0.f, 500.f);

Character::Character(Vector2f velocity, Timeline& timeline) :
	ConvexShape(4), Movable(velocity, timeline), left(nullptr), right(nullptr), up(nullptr), bottom(nullptr)
{
	setPoint(0, Vector2f(30.f, 0.f));
	setPoint(1, Vector2f(60.f, 60.f));
	setPoint(2, Vector2f(30.f, 120.f));
	setPoint(3, Vector2f(0.f, 60.f));

	outVelocity = Vector2f(0.f, 0.f);
}

void Character::update(RenderTarget& target, double thisTime)
{
	// calculate time elapsed
	double elapsed = thisTime - lastTime;

	// calculate total velocity
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{// left
		outVelocity -= velocity;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{// right
		outVelocity += velocity;
	}
	if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) 
		&& bottom != nullptr) // character should be on a platform to jump
	{// jump
		outVelocity.y = -300.f;
	}

	// calculate total displacement
	Vector2f s = Vector2f(outVelocity.x * elapsed, outVelocity.y * elapsed);

	move(s);

	// refresh time
	lastTime = thisTime;
}

void Character::detectCollision(list<MovingPlatform*> platforms, double thisTime)
{
	// calculate four bounding points
	FloatRect box = getGlobalBounds();
	Vector2f 
		leftBound(box.left, box.top + 0.5 * box.height), 
		rightBound(box.left + box.width, box.top + 0.5 * box.height),
		upBound(box.left + 0.5 * box.width, box.top),
		bottomBound(box.left + 0.5 * box.width, box.top + box.height);
	// create rectangles for four points
	Vector2f point(1.f, 1.f);
	RectangleShape l(point), r(point), u(point), b(point);
	l.setPosition(leftBound);
	r.setPosition(rightBound);
	u.setPosition(upBound);
	b.setPosition(bottomBound);

	// clear platforms to null
	left = right = up = bottom = nullptr;

	for (MovingPlatform *platform : platforms)
	{
		// check collision and set out velocity
		if (l.getGlobalBounds().intersects(platform->getGlobalBounds()))
		{// left collision!
			left = platform;
		}
		if (r.getGlobalBounds().intersects(platform->getGlobalBounds()))
		{// right collision!
			right = platform;
		}
		if (u.getGlobalBounds().intersects(platform->getGlobalBounds()))
		{// up collision!
			up = platform;
		}
		if (b.getGlobalBounds().intersects(platform->getGlobalBounds()))
		{// bottom collision!
			bottom = platform;
		}
	}

	// set out velocity
	setOutVelocity(thisTime);
}

void Character::setOutVelocity(double thisTime)
{
	// clear out velocity to normal
	outVelocity.x = 0;

	if (left != nullptr)
	{// left collision!
		outVelocity += velocity;
		outVelocity.x += (left->getHeadingPositive() ?
			left->getVelocity().x : -left->getVelocity().x);
	}

	if (right != nullptr)
	{// right collision!
		outVelocity -= velocity;
		outVelocity.x += (right->getHeadingPositive() ?
			right->getVelocity().x : -right->getVelocity().x);
	}

	if (up != nullptr)
	{// up collision!
		outVelocity.y = 0;
	}

	if (bottom != nullptr)
	{// bottom collision!
		outVelocity.y = 0;
		outVelocity.x += (bottom->getHeadingPositive() ?
			bottom->getVelocity().x : -bottom->getVelocity().x);
	}
	else // drop 
	{
		outVelocity.y += gravity.y * (thisTime - lastTime);
	}
}
