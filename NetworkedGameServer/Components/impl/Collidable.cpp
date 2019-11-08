#include "../../Objects/Character.h"
#include "../../Objects/SideBoundary.h"
#include "../../Objects/SpawnPoint.h"

//void Collidable::platformWork(Collidable* platform, FloatRect bound, 
//	vector<RectangleShape>& boundary_lines, vector<Collidable*>* boundary_ptrs)
//{
//	// check collision using loop
//	for (size_t i = 0; i < boundary_lines.size(); i++) {
//		if (boundary_lines[i].getGlobalBounds().intersects(bound))
//		{// collision!
//			boundary_ptrs->at(i) = platform;
//		}
//	}
//}
//
//void Collidable::deathZoneWork(vector<SpawnPoint*>* spawnPoints)
//{
//	// randomly select a spawn point to respawn
//	int index = rand() % spawnPoints->size();
//	Vector2f point = dynamic_cast<Renderable*>((*spawnPoints)[index]->getGC(ComponentType::RENDERABLE))
//		->getShape()->getPosition();
//	this->getRenderable()->getShape()->setPosition(point);
//}
//
//void Collidable::sideBoundaryWork(Collidable* sideBoundary,
//	Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries)
//{
//	SideBoundary* boundary = dynamic_cast<SideBoundary*>(sideBoundary->getGameObject());
//
//	// add offset this time to overall offset
//	Vector2f offset = boundary->getOffset();
//	renderOffset += offset;
//
//	// update the position of all the sideBoundaries
//	for (SideBoundary* boundary : *sideBoundaries)
//	{
//		boundary->updatePos(offset);
//	}
//}

Collidable::Collidable(GameObject* gameObject, ::Collision collision, Renderable* renderable, Movable* movable)
	: GenericComponent(gameObject), collision(collision), renderable(renderable), movable(movable)
{
}

void Collidable::work(list<Collidable*>& objects, double elapsed,
	Vector2f& renderOffset, vector<SideBoundary*>* sideBoundaries)
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

	// set out velocity
	character->setOutVelocity(elapsed);
}
