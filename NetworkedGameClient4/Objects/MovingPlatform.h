#pragma once
#include "GameObject.h"
#include "../Components/Collidable.h"

class MovingPlatform : public GameObject
{
public:
	MovingPlatform(Renderable* renderable, Movable* movable, Collidable* collidable);

	MovingPlatform(
		::Shape shape, ::Color color, Vector2f size, Vector2f pos,
		Vector2f velocity, Timeline& timeline, ::Move move, float negBound = 0, float range = 0
	);
};

