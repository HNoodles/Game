#pragma once
#include "GameObject.h"
#include "../Components/Collidable.h"


class SpawnPoint : public GameObject
{
public:
	SpawnPoint(Vector2f pos);
};
