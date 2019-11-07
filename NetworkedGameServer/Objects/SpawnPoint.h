#pragma once
#include "GameObject.h"
#include "../Components/Collidable.h"


class SpawnPoint : public GameObject
{
public:
	SpawnPoint(string id, Vector2f pos);
};

