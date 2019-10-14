#pragma once

#include <list>
#include "Renderable.h"

using namespace std;

enum Collision {
	CHARACTER, PLATFORM, DEADZONE, BOUNDARY
};

class Collidable : public GenericComponent
{
private:
	::Collision collision;
	Renderable* render;

	Vector2f* velocity; // for platform
	Vector2f* outVelocity; // for character

public:
	Collidable(::Collision collision, Renderable* render);

	void work(list<Collidable*>& objects, list<Collidable*>& boundary_ptrs);
};

