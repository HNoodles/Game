#include "../../Objects/Invader.h"
#include "../../Objects/InvaderMatrix.h"
#include "../../Objects/Character.h"

void Movable::cMove(double elapsed)
{
	vector<bool> heading = dynamic_cast<Invader*>(gameObject)->getMatrix()->getHeading();

	// calculate displacement
	Vector2f dis = Vector2f((float)(velocity.x * elapsed), (float)(velocity.y * elapsed));

	Vector2f pos = renderable->getShape()->getPosition();

	gameObject->getEM()->insertEvent(
		new EObjMovement(
			gameObject->getEM()->getCurrentTime(),
			gameObject,
			pos.x + dis.x,
			pos.y + dis.y,
			false
		)
	);
}

void Movable::bMove(double elapsed)
{
	// calculate total displacement
	Vector2f dis = Vector2f((float)(velocity.x * elapsed), (float)(velocity.y * elapsed));
	if (dynamic_cast<Bullet*>(getGameObject())->getHeading() == false) // heading up
		dis = -dis;

	Vector2f pos = renderable->getShape()->getPosition();

	gameObject->getEM()->insertEvent(
		new EObjMovement(
			gameObject->getEM()->getCurrentTime(),
			gameObject,
			pos.x + dis.x,
			pos.y + dis.y,
			false
		)
	);
}

void Movable::iMove(double elapsed)
{
	Vector2f outVelocity = dynamic_cast<Character*>(gameObject)->getOutVelocity();

	// calculate total displacement
	Vector2f dis = Vector2f((float)(outVelocity.x * elapsed), (float)(outVelocity.y * elapsed));

	Vector2f pos = renderable->getShape()->getPosition();

	gameObject->getEM()->insertEvent(
		new EObjMovement(
			gameObject->getEM()->getCurrentTime(), 
			gameObject, 
			pos.x + dis.x,
			pos.y + dis.y, 
			false
		)
	);
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
	case Move::CLUSTER:
		cMove(elapsed);
		break;
	case Move::KEYINPUT:
		iMove(elapsed);
		break;
	default:
		break;
	}
}
