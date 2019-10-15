#include "../Character.h"

Character::Character(Renderable* renderable, Movable* movable, Collidable* collidable) 
	: GameObject(), boundary_ptrs({ nullptr, nullptr, nullptr, nullptr })
{
	this->addGC(ComponentType::RENDERABLE, renderable);
	this->addGC(ComponentType::MOVABLE, movable);
	this->addGC(ComponentType::COLLIDABLE, collidable);
}
