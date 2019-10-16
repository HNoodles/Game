#pragma once

#include <list>
#include "Movable.h"

using namespace std;

enum class Collision {
	CHARACTER, PLATFORM, DEATHZONE, BOUNDARY
};

class Collidable : public GenericComponent // in charge of collision related affairs
{
private:
	::Collision collision;
	Renderable* render;
	Movable* movable;
	const Vector2f gravity = Vector2f(0.f, 500.f);

	vector<Collidable*>* boundary_ptrs;// only character has

	void setOutVelocity(double elapsed);

	void platformWork(Collidable* platform, FloatRect bound, vector<RectangleShape>& boundary_lines);
public:
	Collidable(::Collision collision, Renderable* render, Movable* movable, 
		vector<Collidable*>* boundary_ptrs = nullptr);

	void setBoundaryPtrs(vector<Collidable*>* boundary_ptrs)
	{
		this->boundary_ptrs = boundary_ptrs;
	}

	void work(list<Collidable*>& objects, double elapsed);
};

