#pragma once

#include "../Times/Timeline.h"
#include "../Times/GameTime.h"
#include "Renderable.h"

class Invader;
class Character;

enum class Move {
	STATIC, CLUSTER, BULLET, KEYINPUT
};

class Movable : public GenericComponent // in charge of moving related things
{
private:
	Renderable* renderable;
	Vector2f velocity;
	Timeline& timeline;
	::Move move;

	void cMove(double elapsed);

	void bMove(double elapsed);

	void iMove(double elapsed);
public:
	Movable(GameObject* gameObject, Renderable* renderable,
		Vector2f velocity, Timeline& timeline, ::Move move, float negBound = 0, float range = 0); // for movable

	Vector2f getVelocity() const
	{
		return velocity;
	}

	Timeline& getTimeline()
	{
		return timeline;
	}

	void work(double elapsed);
};