#include "../SpawnPoint.h"

SpawnPoint::SpawnPoint(string id, Vector2f pos) : GameObject(id)
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, ::Shape::CIRCLE, ::Color::TRANS_PARENT, Vector2f(1.f, 1.f), pos)
	);
}
