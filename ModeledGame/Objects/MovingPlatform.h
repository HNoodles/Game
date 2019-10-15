#pragma once
#include "GameObject.h"
#include "../Components/Renderable.h"
#include "../Components/Movable.h"
#include "../Components/Collidable.h"

class MovingPlatform : public GameObject
{
public:
	MovingPlatform(Renderable renderable, Movable movable, Collidable collidable);
};

