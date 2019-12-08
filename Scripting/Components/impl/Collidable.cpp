#include "../../Objects/Character.h"
#include "../../Objects/SideBoundary.h"
#include "../../Objects/SpawnPoint.h"

Collidable::Collidable(GameObject* gameObject, ::Collision collision, Renderable* renderable, Movable* movable)
	: GenericComponent(gameObject), collision(collision), renderable(renderable), movable(movable)
{
}

void Collidable::work(list<Collidable*>& objects, double elapsed)
{
	// get four boundaries
	vector<RectangleShape> boundary_lines = renderable->getBoundaryLines();

	Character* character = dynamic_cast<Character*>(gameObject);

	// reset all bdry ptrs to null
	vector<Collidable*>* boundary_ptrs = character->getBoundaryPtrs();
	for (size_t i = 0; i < boundary_ptrs->size(); i++) {
		boundary_ptrs->at(i) = nullptr;
	}

	for (Collidable* object : objects)
	{
		// get bound of character and object
		FloatRect cbound = renderable->getShape()->getGlobalBounds();
		FloatRect bound = object->renderable->getShape()->getGlobalBounds();

		if (cbound.intersects(bound)) {// collision happens
			// generate character collision event in event manager
			gameObject->getEM()->insertEvent(
				new ECharCollision(
					gameObject->getEM()->getCurrentTime(), 
					character, 
					object->getGameObject()
				)
			);
		}
	}

	// reset out velocity x
	character->getOutVelocity().x = 0.f;
}
