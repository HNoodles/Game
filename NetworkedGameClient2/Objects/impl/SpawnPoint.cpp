#include "../SpawnPoint.h"

SpawnPoint::SpawnPoint(Vector2f pos) : GameObject()
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, ::Shape::CIRCLE, ::Color::TRANS_PARENT, Vector2f(1.f, 1.f), pos)
	);
}
