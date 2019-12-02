#include "../Bullet.h"

Bullet::Bullet(string id, EventManager* manager, Vector2f pos, Timeline& timeline) : 
	GameObject(id, manager)
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, ::Shape::DIAMOND, ::Color::RED, Vector2f(1.f, 2.f), pos)
	);
	this->addGC(
		ComponentType::MOVABLE,
		new Movable(
			this,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			Vector2f(0.f, 100.f), timeline, ::Move::STATIC
		)
	);
	this->addGC(
		ComponentType::COLLIDABLE,
		new Collidable(
			this,
			Collision::BULLET,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))
		)
	);
}
