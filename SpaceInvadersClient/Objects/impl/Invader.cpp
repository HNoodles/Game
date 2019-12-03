#include "../Invader.h"
#include "../InvaderMatrix.h"

Invader::Invader(string id, EventManager* manager, 
	Vector2f pos, Vector2f velocity, Timeline& timeline, InvaderMatrix* matrix) :
	GameObject(id, manager), matrix(matrix)//, alive(true)
{
	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, ::Shape::DIAMOND, ::Color::RED, Vector2f(20.f, 40.f), pos)
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

Bullet* Invader::fire(int roundCount)
{
	Renderable* renderable = dynamic_cast<Renderable*>(getGC(ComponentType::RENDERABLE));
	Vector2f pos = renderable->getShape()->getPosition();
	Vector2f size = renderable->getSize();

	Vector2f bulletPos = Vector2f(pos.x + 1 / 2 * size.x, pos.y);
	
	Timeline& timeline = dynamic_cast<Movable*>(getGC(ComponentType::MOVABLE))->getTimeline();

	return new Bullet(getId() + to_string(roundCount), getEM(), bulletPos, timeline, true);
}
