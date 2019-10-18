#pragma once

#include <list>
#include "Movable.h"

using namespace std;

enum class Collision {
	CHARACTER, PLATFORM, DEATHZONE, SIDEBOUNDARY
};

class Collidable : public GenericComponent // in charge of collision related affairs
{
private:
	::Collision collision;
	Renderable* renderable;
	Movable* movable;
	const Vector2f gravity = Vector2f(0.f, 500.f);

	// only character has
	vector<Collidable*>* boundary_ptrs;
	vector<Renderable*>* spawnPoints;

	void setOutVelocity(double elapsed);

	void platformWork(Collidable* platform, FloatRect bound, vector<RectangleShape>& boundary_lines);

	void deathZoneWork();

	void sideBoundaryWork(Collidable* sideBoundary);
public:
	Collidable(void* gameObject, ::Collision collision, Renderable* renderable, Movable* movable);

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

	void setBoundaryPtrs(vector<Collidable*>* boundary_ptrs)
	{
		this->boundary_ptrs = boundary_ptrs;
	}

	void setSpawnPoints(vector<Renderable*>* spawnPoints)
	{
		this->spawnPoints = spawnPoints;
	}

	/*void setOffset(Vector2f offset)
	{
		this->offset = offset;
	}

	Vector2f getOffset() const
	{
		return offset;
	}

	void setDirection(::Direction direction)
	{
		this->direction = direction;
	}

	::Direction getDirection() const
	{
		return direction;
	}*/

	void work(list<Collidable*>& objects, double elapsed);
		//Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries);
};

