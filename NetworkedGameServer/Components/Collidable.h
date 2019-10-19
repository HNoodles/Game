#pragma once

#include <list>
#include "Movable.h"

using namespace std;

class Character;
class SideBoundary;

enum class Collision {
	CHARACTER, PLATFORM, DEATHZONE, SIDEBOUNDARY
};

class Collidable : public GenericComponent // in charge of collision related affairs
{
private:
	::Collision collision;
	Renderable* renderable;
	Movable* movable;

	void platformWork(Collidable* platform, FloatRect bound, vector<RectangleShape>& boundary_lines, vector<Collidable*>* boundary_ptrs);

	void deathZoneWork(vector<Renderable*>* spawnPoints);

	void sideBoundaryWork(Collidable* sideBoundary,
		Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries);
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

