#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Movable
{
protected:
	Vector2f velocity;
public:
	Vector2f getVelocity() const
	{
		return velocity;
	}

	void setVelocity(Vector2f v) 
	{
		velocity = v;
	}

	virtual void around(RenderTarget& target) = 0;
};