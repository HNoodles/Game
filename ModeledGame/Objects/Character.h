#pragma once
#include <vector>
#include "GameObject.h"
#include "../Components/Renderable.h"
#include "../Components/Movable.h"
#include "../Components/Collidable.h"

class Character : public GameObject
{
private:
	vector<Collidable*> boundary_ptrs;
public:
	Character(Renderable renderable, Movable movable, Collidable collidable);

	vector<Collidable*>* getBoundaryPtrs()
	{
		return &boundary_ptrs;
	}
};