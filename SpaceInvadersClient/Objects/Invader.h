#pragma once
#include "GameObject.h"
#include "Bullet.h"

class InvaderMatrix;

class Invader : public GameObject
{
private:
	InvaderMatrix* matrix;
	//bool alive;
public:
	Invader(
		string id, EventManager* manager,
		Vector2f pos, Vector2f velocity, Timeline& timeline, InvaderMatrix* matrix
	);

	/*void killed()
	{
		alive = false;
	}*/

	Bullet* fire(int roundCount);

	InvaderMatrix* getMatrix()
	{
		return matrix;
	}
};

