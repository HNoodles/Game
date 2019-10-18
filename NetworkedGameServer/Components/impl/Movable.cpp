#include "../Movable.h"

void Movable::hMove(double elapsed)
{
	sf::Shape* s = renderable->getShape();
	Vector2f& size = renderable->getSize();

	Vector2f pos = s->getPosition();

	// calculate displacement
	Vector2f dis = Vector2f((float)(velocity.x * elapsed), (float)(velocity.y * elapsed));

	// move object
	if ((headingPositive && pos.x <= posBound)
		|| (!headingPositive && pos.x <= negBound))
	{// heading right, not reaching bound
	// heading left, reached bound
		headingPositive = true;
		s->move(dis);
	}
	else if ((headingPositive && pos.x > posBound)
		|| (!headingPositive && pos.x > negBound))
	{// heading right, reached bound
	// heading left, not reaching bound
		headingPositive = false;
		s->move(-dis);
	}
}

void Movable::vMove(double elapsed)
{
	sf::Shape* s = renderable->getShape();
	Vector2f& size = renderable->getSize();

	Vector2f pos = s->getPosition();

	// calculate displacement
	Vector2f dis = Vector2f((float)(velocity.x * elapsed), (float)(velocity.y * elapsed));

	// move object
	if ((headingPositive && pos.y <= posBound)
		|| (!headingPositive && pos.y <= negBound))
	{// heading down, not reaching bound
	// heading up, reached bound
		headingPositive = true;
		s->move(dis);
	}
	else if ((headingPositive && pos.y > posBound)
		|| (!headingPositive && pos.y > negBound))
	{// heading down, reached bound
	// heading up, not reaching bound
		headingPositive = false;
		s->move(-dis);
	}
}

void Movable::iMove(double elapsed)
{
	// calculate total displacement
	Vector2f dis = Vector2f((float)(outVelocity.x * elapsed), (float)(outVelocity.y * elapsed));

	renderable->getShape()->move(dis);
}

Movable::Movable(void* gameObject, Renderable* renderable,
	Vector2f velocity, Timeline& timeline, ::Move move, float negBound, float range) // for movable 
	: GenericComponent(gameObject), renderable(renderable), 
	velocity(velocity), timeline(timeline), move(move), 
	headingPositive(false), negBound(negBound), posBound(negBound + range), // for platforms
	outVelocity(0.f, 0.f), jumpable(false) // for character
{
}

void Movable::handleKeyInput()
{
	// only character can call this function
	if (move != Move::KEYINPUT)
		return;

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
}

void Movable::work(double elapsed)
{
	switch (move)
	{
	case Move::HORIZONTAL:
		hMove(elapsed);
		break;
	case Move::VERTICAL:
		vMove(elapsed);
		break;
	case Move::KEYINPUT:
		iMove(elapsed);
		break;
	default:
		break;
	}
}
