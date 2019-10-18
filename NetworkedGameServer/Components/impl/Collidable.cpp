#include "../Collidable.h"

void Collidable::setOutVelocity(double elapsed)
{
	Vector2f velocity = movable->getVelocity();
	Vector2f& outVelocity = movable->getOutVelocity();

	Collidable
		* left = boundary_ptrs->at(0),
		* right = boundary_ptrs->at(1),
		* up = boundary_ptrs->at(2),
		* bottom = boundary_ptrs->at(3);

	// clear out velocity to normal
	outVelocity.x = 0;

	if (left != nullptr)
	{// left collision!
		Movable* lmovable = left->movable;
		Vector2f lv = lmovable->getVelocity();
		outVelocity.x = lmovable->getHeadingPositive() ? lv.x : -lv.x;
	}

	if (right != nullptr)
	{// right collision!
		Movable* rmovable = right->movable;
		Vector2f rv = rmovable->getVelocity();
		outVelocity.x = rmovable->getHeadingPositive() ? rv.x : -rv.x;
	}

	if (up != nullptr)
	{// up collision!
		outVelocity.y = 0;
	}

	if (bottom != nullptr)
	{// bottom collision!
		movable->setJumpable(true);

		Movable* bmovable = bottom->movable;
		Vector2f bv = bmovable->getVelocity();
		// only when no blocks on the heading side can character get velocity.x from bottom
		if ((left == nullptr && !bmovable->getHeadingPositive()) 
			|| (right == nullptr && bmovable->getHeadingPositive()))
			outVelocity.x = bmovable->getHeadingPositive() ? bv.x : -bv.x;
		outVelocity.y = bmovable->getHeadingPositive() ? bv.y : -bv.y;
	}
	else // drop 
	{
		movable->setJumpable(false);
		outVelocity.y += (float)(gravity.y * elapsed);
	}
}

void Collidable::platformWork(Collidable* platform, FloatRect bound, vector<RectangleShape>& boundary_lines)
{
	// check collision using loop
	for (size_t i = 0; i < boundary_lines.size(); i++) {
		if (boundary_lines[i].getGlobalBounds().intersects(bound))
		{// collision!
			boundary_ptrs->at(i) = platform;
		}
	}
}

void Collidable::deathZoneWork()
{
	// randomly select a spawn point to respawn
	int index = rand() % spawnPoints->size();
	Vector2f point = (*spawnPoints)[index]->getShape()->getPosition();
	this->getRenderable()->getShape()->setPosition(point);
}

void Collidable::sideBoundaryWork(Collidable* sideBoundary)
{
	//// add offset this time to overall offset
	//renderOffset += sideBoundary->getOffset();

	//// update the position of all the sideBoundaries
	//for (SideBoundary* boundary : *sideBoundaries)
	//{
	//	boundary->updatePos(sideBoundary->getDirection());
	//}
}

Collidable::Collidable(void* gameObject, ::Collision collision, Renderable* renderable, Movable* movable)
	: GenericComponent(gameObject), collision(collision), renderable(renderable), movable(movable), 
	boundary_ptrs(nullptr), spawnPoints(nullptr)
{
}

void Collidable::work(list<Collidable*>& objects, double elapsed)
	//Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries)
{
	// calculate four boundaries
	FloatRect cbound = renderable->getShape()->getGlobalBounds();
	Vector2f height(1.f, cbound.height), width(cbound.width, 1.f);

	RectangleShape l(height), r(height), u(width), b(width);
	l.setPosition(Vector2f(cbound.left, cbound.top));
	r.setPosition(Vector2f(cbound.left + cbound.width, cbound.top));
	u.setPosition(Vector2f(cbound.left, cbound.top));
	b.setPosition(Vector2f(cbound.left, cbound.top + cbound.height));

	vector<RectangleShape> boundary_lines({ l, r, u, b });

	// reset all bdry ptrs to null
	for (size_t i = 0; i < boundary_ptrs->size(); i++) {
		boundary_ptrs->at(i) = nullptr;
	}

	for (Collidable* object : objects)
	{
		// get bound of object
		FloatRect bound = object->renderable->getShape()->getGlobalBounds();

		if (cbound.intersects(bound)) {// collision happens
			switch (object->collision)
			{
			case Collision::PLATFORM:
				platformWork(object, bound, boundary_lines);
				break;
			case Collision::DEATHZONE: 
				deathZoneWork();
				break;
			case Collision::SIDEBOUNDARY: 
				sideBoundaryWork(object);
				break;
			default:
				break;
			}
		}
	}

	// set out velocity
	setOutVelocity(elapsed);
}
