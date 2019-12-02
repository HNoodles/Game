#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
private:
	bool heading;
public:
	Bullet(
		string id, EventManager* manager,
		Vector2f pos, Timeline& timeline, bool heading
	);

	bool getHeading() const
	{
		return heading;
	}
};

