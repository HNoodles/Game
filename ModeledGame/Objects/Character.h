#pragma once
#include <vector>
#include "GameObject.h"
#include "../Components/Collidable.h"

class Character : public GameObject
{
private:
	vector<Collidable*> boundary_ptrs;
public:
	Character(Renderable* renderable, Movable* movable, Collidable* collidable);

	Character(
		::Shape shape, ::Color color, Vector2f size, Vector2f pos, 
		Vector2f velocity, Timeline& timeline
	);

	vector<Collidable*>* getBoundaryPtrs()
	{
		return &boundary_ptrs;
	}
};