#pragma once
#include "GameObject.h"
#include "../Components/Collidable.h"


class DeathZone : public GameObject
{
public:
	DeathZone(::Shape shape, Vector2f size, Vector2f pos);
};

