#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>

#include "MovingPlatform.h"

using namespace sf;
using namespace std;


class Character : public ConvexShape, public Movable, 
{
private:
	MovingPlatform *left, *right, *up, *bottom;
	Vector2f outVelocity;
	Vector2f dy;

	void setOutVelocity(double elapsed);
public:
	Character(Vector2f velocity, Timeline& timeline);

	void update(double elapsed) override;

	void handleKeyInput();

	void detectCollision(list<MovingPlatform*> collidableObjects, double elapsed);
};