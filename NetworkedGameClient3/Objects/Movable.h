#pragma once

#include <SFML/Graphics.hpp>
#include "../Times/Timeline.h"
#include "../Times/GameTime.h"

using namespace sf;

class Movable, 
{
protected:
	Vector2f velocity;
	Timeline& timeline;
public:
	Movable, (Vector2f velocity, Timeline& timeline);

	Vector2f getVelocity() const
	{
		return velocity;
	}

	void setVelocity(Vector2f v) 
	{
		velocity = v;
	}

	void setTimeline(Timeline& timeline)
	{
		this->timeline = timeline;
	}

	Timeline& getTimeline() const
	{
		return timeline;
	}

	virtual void update(double elapsed) = 0;
};