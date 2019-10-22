#include "../../Objects/MovingPlatform.h"
#include "../../Objects/Character.h"

void Movable::hMove(double elapsed)
{
	sf::Shape* s = renderable->getShape();
	Vector2f& size = renderable->getSize();
	
	MovingPlatform* platform = dynamic_cast<MovingPlatform*>(gameObject);
	bool headingPositive = platform->getHeadingPositive();
	float posBound = platform->getPosBound();
	float negBound = platform->getNegBound();

	Vector2f pos = s->getPosition();

	// calculate displacement
	Vector2f dis = Vector2f((float)(velocity.x * elapsed), (float)(velocity.y * elapsed));

	// move object
	if ((headingPositive && pos.x <= posBound)
		|| (!headingPositive && pos.x <= negBound))
	{// heading right, not reaching bound
	// heading left, reached bound
		platform->setHeadingPositive(true);
		s->move(dis);
	}
	else if ((headingPositive && pos.x > posBound)
		|| (!headingPositive && pos.x > negBound))
	{// heading right, reached bound
	// heading left, not reaching bound
		platform->setHeadingPositive(false);
		s->move(-dis);
	}
}

void Movable::vMove(double elapsed)
{
	sf::Shape* s = renderable->getShape();
	Vector2f& size = renderable->getSize();

	MovingPlatform* platform = dynamic_cast<MovingPlatform*>(gameObject);
	bool headingPositive = platform->getHeadingPositive();
	float posBound = platform->getPosBound();
	float negBound = platform->getNegBound();

	Vector2f pos = s->getPosition();

	// calculate displacement
	Vector2f dis = Vector2f((float)(velocity.x * elapsed), (float)(velocity.y * elapsed));

	// move object
	if ((headingPositive && pos.y <= posBound)
		|| (!headingPositive && pos.y <= negBound))
	{// heading down, not reaching bound
	// heading up, reached bound
		platform->setHeadingPositive(true);
		s->move(dis);
	}
	else if ((headingPositive && pos.y > posBound)
		|| (!headingPositive && pos.y > negBound))
	{// heading down, reached bound
	// heading up, not reaching bound
		platform->setHeadingPositive(false);
		s->move(-dis);
	}
}

void Movable::iMove(double elapsed)
{
	Vector2f outVelocity = dynamic_cast<Character*>(gameObject)->getOutVelocity();

	// calculate total displacement
	Vector2f dis = Vector2f((float)(outVelocity.x * elapsed), (float)(outVelocity.y * elapsed));

	renderable->getShape()->move(dis);
}

Movable::Movable(GameObject* gameObject, Renderable* renderable,
	Vector2f velocity, Timeline& timeline, ::Move move, float negBound, float range) // for movable 
	: GenericComponent(gameObject), renderable(renderable), 
	velocity(velocity), timeline(timeline), move(move)
{
}

void Movable::work(double elapsed)
{
	switch (move)
	{
	case Move::STATIC:
		// do nothing
		break;
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
