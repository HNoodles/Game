#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(
		string id, EventManager* manager,
		Vector2f pos, Timeline& timeline
	);
};

