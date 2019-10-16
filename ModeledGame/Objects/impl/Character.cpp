#include "../Character.h"

Character::Character(Renderable* renderable, Movable* movable, Collidable* collidable) 
	: GameObject(), boundary_ptrs({ nullptr, nullptr, nullptr, nullptr })
{
	this->addGC(ComponentType::RENDERABLE, renderable);
	this->addGC(ComponentType::MOVABLE, movable);
	this->addGC(ComponentType::COLLIDABLE, collidable);
}

Character::Character(::Shape shape, ::Color color, Vector2f size, Vector2f pos, Vector2f velocity, Timeline& timeline)
	: GameObject(), boundary_ptrs({ nullptr, nullptr, nullptr, nullptr })
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
	// set boundary ptrs to collidable
	dynamic_cast<Collidable*>(this->getGC(ComponentType::COLLIDABLE))->setBoundaryPtrs(&boundary_ptrs);
}
