#pragma once

#include "GenericComponent.h"
#include "../Times/Timeline.h"
#include "../Times/GameTime.h"

enum Move {
	HORIZONTAL, VERTICAL, INPUT
};

class Movable : public GenericComponent // incharge of moving related things
{
private:
	sf::Shape* s;
	Vector2f& size;
	Vector2f& velocity;
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
protected:
	bool jumpable;
public:
	Movable(sf::Shape* s, Vector2f& size, 
		Vector2f& velocity, Timeline& timeline, ::Move move, float negBound = 0, float range = 0); // for movable

	Vector2f getVelocity() const
	{
		return velocity;
	}

	/*void setVelocity(Vector2f v) 
	{
		velocity = v;
	}*/

	Vector2f& getOutVelocity()
	{
		return outVelocity;
	}

	bool getHeadingPositive() const
	{
		return headingPositive;
	}

	void work(double elapsed);
};