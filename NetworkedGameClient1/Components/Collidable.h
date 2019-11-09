#pragma once

#include <list>
#include "Movable.h"

class Character;
class SideBoundary;
class SpawnPoint;

enum class Collision {
	CHARACTER, PLATFORM, DEATHZONE, SIDEBOUNDARY
};

class Collidable : public GenericComponent // in charge of collision related affairs
{
private:
	::Collision collision;
	Renderable* renderable;
	Movable* movable;
public:
	Collidable(GameObject* gameObject, ::Collision collision, Renderable* renderable, Movable* movable);

	::Collision getType() const
	{
		return collision;
	}

	Renderable* getRenderable()
	{
		return renderable;
	}

	Movable* getMovable()
	{
		return movable;
	}

	void work(list<Collidable*>& objects, double elapsed, 
		Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries);
};

