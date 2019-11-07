#include "../MovingPlatform.h"

MovingPlatform::MovingPlatform(string id, ::Shape shape, ::Color color, Vector2f size, 
	Vector2f pos, Vector2f velocity, Timeline& timeline, ::Move move, float negBound, float range)
	: GameObject(id), headingPositive(false), negBound(negBound), posBound(negBound + range)
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, shape, color, size, pos)
	);
	this->addGC(
		ComponentType::MOVABLE,
		new Movable(
			this, 
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			velocity, timeline, move, negBound, range
		)
	);
	this->addGC(
		ComponentType::COLLIDABLE,
		new Collidable(
			this, 
			Collision::PLATFORM,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))
		)
	);
}
