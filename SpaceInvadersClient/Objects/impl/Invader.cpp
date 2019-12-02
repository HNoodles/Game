#include "../Invader.h"
#include "../InvaderMatrix.h"

Invader::Invader(string id, EventManager* manager, 
	Vector2f pos, Vector2f velocity, Timeline& timeline, InvaderMatrix* matrix) :
	GameObject(id, manager), matrix(matrix), alive(true)
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, ::Shape::DIAMOND, ::Color::RED, Vector2f(10.f, 20.f), pos)
	);
	this->addGC(
		ComponentType::MOVABLE,
		new Movable(
			this,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			velocity, timeline, ::Move::CLUSTER
		)
	);
	this->addGC(
		ComponentType::COLLIDABLE,
		new Collidable(
			this,
			Collision::INVADER,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))
		)
	);
}
