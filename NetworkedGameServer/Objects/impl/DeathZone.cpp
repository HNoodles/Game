#include "../DeathZone.h"

DeathZone::DeathZone(string id, ::Shape shape, Vector2f size, Vector2f pos) : GameObject(id)
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, shape, ::Color::TRANS_PARENT, size, pos)
	);
	this->addGC(
		ComponentType::COLLIDABLE,
		new Collidable(
			this, 
			Collision::DEATHZONE,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			nullptr
		)
	);
}
