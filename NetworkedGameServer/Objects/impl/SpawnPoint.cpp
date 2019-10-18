#include "../SpawnPoint.h"

SpawnPoint::SpawnPoint(Vector2f pos) : GameObject()
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(::Shape::CIRCLE, ::Color::TRANSPARENT, Vector2f(1.f, 1.f), pos)
	);
}
