#include "../Character.h"

Character::Character(::Shape shape, ::Color color, Vector2f size, Vector2f pos, Vector2f velocity, Timeline& timeline, vector<Renderable*>* spawnPoints)
	: GameObject(), boundary_ptrs({ nullptr, nullptr, nullptr, nullptr }), spawnPoints(spawnPoints)
{
	this->addGC(
		ComponentType::RENDERABLE, 
		new Renderable(shape, color, size, pos)
	);
	this->addGC(
		ComponentType::MOVABLE, 
		new Movable(
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)), 
			velocity, timeline, Move::KEYINPUT
		)
	);
	this->addGC(
		ComponentType::COLLIDABLE, 
		new Collidable(
			Collision::CHARACTER, 
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)), 
			dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))
		)
	);

	// set boundary ptrs and spawn points to collidable
	Collidable* collidable = dynamic_cast<Collidable*>(this->getGC(ComponentType::COLLIDABLE));
	collidable->setBoundaryPtrs(&boundary_ptrs);
	collidable->setSpawnPoints(spawnPoints);
}
