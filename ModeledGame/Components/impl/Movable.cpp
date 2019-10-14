#include "../Movable.h"

void Movable::hMove(double elapsed)
{
	Vector2f pos = s->getPosition();

	// calculate displacement
	Vector2f dis = Vector2f(velocity.x * elapsed, velocity.y * elapsed);

	// move object
	if ((headingPositive && pos.x + size.x <= posBound)
		|| (!headingPositive && pos.x <= negBound))
	{// heading right, not reaching bound
	// heading left, reached bound
		headingPositive = true;
		s->move(dis);
	}
	else if ((headingPositive && pos.x + size.x > posBound)
		|| (!headingPositive && pos.x > negBound))
	{// heading right, reached bound
	// heading left, not reaching bound
		headingPositive = false;
		s->move(-dis);
	}
}

void Movable::vMove(double elapsed)
{
	Vector2f pos = s->getPosition();

	// calculate displacement
	Vector2f dis = Vector2f(velocity.x * elapsed, velocity.y * elapsed);

	// move object
	if ((headingPositive && pos.y + size.y <= posBound)
		|| (!headingPositive && pos.y <= negBound))
	{// heading down, not reaching bound
	// heading up, reached bound
		headingPositive = true;
		s->move(dis);
	}
	else if ((headingPositive && pos.y + size.y > posBound)
		|| (!headingPositive && pos.y > negBound))
	{// heading down, reached bound
	// heading up, not reaching bound
		headingPositive = false;
		s->move(-dis);
	}
}

void Movable::iMove(double elapsed)
{
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
		&& jumpable) // character should be on a platform to jump
	{// jump
		outVelocity.y = -300.f;
	}

	// calculate total displacement
	Vector2f dis = Vector2f(outVelocity.x * elapsed, outVelocity.y * elapsed);

	s->move(dis);
}

Movable::Movable(sf::Shape* s, Vector2f& size,
	Vector2f& velocity, Timeline& timeline, ::Move move, float negBound = 0, float range = 0) // for movable 
	: s(s), size(size), 
	velocity(velocity), timeline(timeline), move(move), 
	headingPositive(true), // for platforms
	outVelocity(0.f, 0.f), jumpable(false) // for character
{
	switch (move)
	{
	case HORIZONTAL:
	case VERTICAL: // set bound
		this->negBound = negBound;
		this->posBound = negBound + range;
		break;
	case INPUT: // do nothing
		break;
	default:
		break;
	}
}

void Movable::work(double elapsed)
{
	switch (move)
	{
	case HORIZONTAL:
		hMove(elapsed);
		break;
	case VERTICAL:
		vMove(elapsed);
		break;
	case INPUT:
		iMove(elapsed);
		break;
	default:
		break;
	}
}
