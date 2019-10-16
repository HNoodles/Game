#pragma once

#include "GenericComponent.h"
#include "../Times/Timeline.h"
#include "../Times/GameTime.h"
#include "Renderable.h"

enum class Move {
	HORIZONTAL, VERTICAL, KEYINPUT
};

class Movable : public GenericComponent // in charge of moving related things
{
private:
	Renderable* renderable;
	Vector2f velocity;
	Timeline& timeline;
	::Move move;

	// for moving platform
	bool headingPositive;
	float negBound, posBound;
	void hMove(double elapsed);
	void vMove(double elapsed);

	// for character
	Vector2f outVelocity;
	void iMove(double elapsed);
	bool jumpable;
public:
	Movable(Renderable* renderable, 
		Vector2f velocity, Timeline& timeline, ::Move move, float negBound = 0, float range = 0); // for movable

	Vector2f getVelocity() const
	{
		return velocity;
	}

	Vector2f& getOutVelocity()
	{
		return outVelocity;
	}

	bool getHeadingPositive() const
	{
		return headingPositive;
	}

	void setJumpable(bool jumpable)
	{
		this->jumpable = jumpable;
	}

	void work(double elapsed);
};