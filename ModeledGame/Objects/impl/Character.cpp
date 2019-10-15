#include "../Character.h"

Character::Character(Renderable renderable, Movable movable, Collidable collidable) 
	: GameObject(), boundary_ptrs({ nullptr, nullptr, nullptr, nullptr })
{
	this->addGC(RENDERABLE, &renderable);
	this->addGC(MOVABLE, &movable);
	this->addGC(COLLIDABLE, &collidable);
}
