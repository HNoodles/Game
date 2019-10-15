#pragma once
#include "GameObject.h"
#include "../Components/Collidable.h"

class MovingPlatform : public GameObject
{
public:
	MovingPlatform(Renderable* renderable, Movable* movable, Collidable* collidable);
};

