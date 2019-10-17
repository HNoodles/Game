#include "../MovingPlatform.h"

MovingPlatform::MovingPlatform(Renderable* renderable, Movable* movable, Collidable* collidable) 
	: GameObject()
{
	this->addGC(ComponentType::RENDERABLE, renderable);
	this->addGC(ComponentType::MOVABLE, movable);
	this->addGC(ComponentType::COLLIDABLE, collidable);
}

MovingPlatform::MovingPlatform(::Shape shape, ::Color color, Vector2f size, Vector2f pos, 
	Vector2f velocity, Timeline& timeline, ::Move move, float negBound, float range)
	: GameObject()
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(shape, color, size, pos)
	);
	this->addGC(
		ComponentType::MOVABLE,
		new Movable(
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			velocity, timeline, move, negBound, range
		)
	);
	this->addGC(
		ComponentType::COLLIDABLE,
		new Collidable(
			Collision::PLATFORM,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))
		)
	);
}
