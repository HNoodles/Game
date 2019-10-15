#include "../MovingPlatform.h"

MovingPlatform::MovingPlatform(Renderable* renderable, Movable* movable, Collidable* collidable) 
	: GameObject()
{
	this->addGC(ComponentType::RENDERABLE, renderable);
	this->addGC(ComponentType::MOVABLE, movable);
	this->addGC(ComponentType::COLLIDABLE, collidable);
}
