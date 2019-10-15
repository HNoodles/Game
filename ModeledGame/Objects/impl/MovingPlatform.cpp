#include "../MovingPlatform.h"

MovingPlatform::MovingPlatform(Renderable renderable, Movable movable, Collidable collidable) 
	: GameObject()
{
	this->addGC(RENDERABLE, &renderable);
	this->addGC(MOVABLE, &movable);
	this->addGC(COLLIDABLE, &collidable);
}
