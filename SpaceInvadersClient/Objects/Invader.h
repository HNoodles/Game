#pragma once
#include "GameObject.h"
#include "Bullet.h"

class InvaderMatrix;

class Invader : public GameObject
{
private:
	InvaderMatrix* matrix;
public:
	Invader(
		string id, EventManager* manager,
		Vector2f pos, Vector2f velocity, Timeline& timeline, InvaderMatrix* matrix
	);

	Bullet* fire(int bulletCount);

	InvaderMatrix* getMatrix()
	{
		return matrix;
	}
};

