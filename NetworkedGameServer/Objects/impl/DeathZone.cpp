#include "../DeathZone.h"

DeathZone::DeathZone(::Shape shape, Vector2f size, Vector2f pos) : GameObject()
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(shape, ::Color::TRANSPARENT, size, pos)
	);
	this->addGC(
		ComponentType::COLLIDABLE,
		new Collidable(
			Collision::DEATHZONE,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			nullptr
		)
	);
}
