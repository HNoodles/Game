#pragma once
#include "GameObject.h"


class SpawnPoint : public GameObject
{
public:
	SpawnPoint(string id, EventManager* manager, Vector2f pos);
};

