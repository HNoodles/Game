#include "../../Objects/Character.h"
#include "../../Objects/SideBoundary.h"

void Collidable::platformWork(Collidable* platform, FloatRect bound, 
	vector<RectangleShape>& boundary_lines, vector<Collidable*>* boundary_ptrs)
{
	// check collision using loop
	for (size_t i = 0; i < boundary_lines.size(); i++) {
		if (boundary_lines[i].getGlobalBounds().intersects(bound))
		{// collision!
			boundary_ptrs->at(i) = platform;
		}
	}
}

void Collidable::deathZoneWork(vector<Renderable*>* spawnPoints)
{
	// randomly select a spawn point to respawn
	int index = rand() % spawnPoints->size();
	Vector2f point = (*spawnPoints)[index]->getShape()->getPosition();
	this->getRenderable()->getShape()->setPosition(point);
}

void Collidable::sideBoundaryWork(Collidable* sideBoundary,
	Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries)
{
	SideBoundary* boundary = dynamic_cast<SideBoundary*>(sideBoundary->getGameObject());

	// add offset this time to overall offset
	Vector2f offset = boundary->getOffset();
	renderOffset += offset;

	// update the position of all the sideBoundaries
	for (SideBoundary* boundary : *sideBoundaries)
	{
		boundary->updatePos(offset);
	}
}

Collidable::Collidable(GameObject* gameObject, ::Collision collision, Renderable* renderable, Movable* movable)
	: GenericComponent(gameObject), collision(collision), renderable(renderable), movable(movable)
{
}

void Collidable::work(list<Collidable*>& objects, double elapsed, 
	Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries)
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

	Character* character = dynamic_cast<Character*>(gameObject);

	// reset all bdry ptrs to null
	vector<Collidable*>* boundary_ptrs = character->getBoundaryPtrs();
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
				platformWork(object, bound, boundary_lines, boundary_ptrs);
				break;
			case Collision::DEATHZONE: 
				deathZoneWork(character->getSpawnPoints());
				break;
			case Collision::SIDEBOUNDARY: 
				sideBoundaryWork(object, renderOffset, sideBoundaries);
				break;
			default:
				break;
			}
		}
	}

	// set out velocity
	character->setOutVelocity(elapsed);
}
