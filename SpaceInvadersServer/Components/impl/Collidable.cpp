#include "../../Objects/Character.h"
#include "../../Objects/Invader.h"

Collidable::Collidable(GameObject* gameObject, ::Collision collision, Renderable* renderable, Movable* movable)
	: GenericComponent(gameObject), collision(collision), renderable(renderable), movable(movable)
{
}

void Collidable::work(list<Collidable*>& objects, double elapsed) // only character and invader would call this func
{
	bool isCharacter = gameObject->getId().find(SELF_NAME) == 0;

	for (Collidable* object : objects)
	{
		// get bound of character and object
		FloatRect cbound = renderable->getShape()->getGlobalBounds();
		FloatRect bound = object->renderable->getShape()->getGlobalBounds();

		if (cbound.intersects(bound)) {// collision happens
			if (isCharacter)
			{// collides with bullet or invader, die
				// generate character death event in event manager
				gameObject->getEM()->insertEvent(
					new ECharDeath(
						gameObject->getEM()->getCurrentTime(),
						dynamic_cast<Character*>(gameObject)
					)
				);
			}
			else // is invader
			{// collides with bullet, die
				gameObject->getEM()->insertEvent(
					new EInvaDeath(
						gameObject->getEM()->getCurrentTime(),
						dynamic_cast<Invader*>(gameObject)
					)
				);
			}
		}
	}
}
