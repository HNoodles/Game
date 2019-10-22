#include "../SideBoundary.h"

SideBoundary::SideBoundary(::Direction direction, Vector2u windowSize, float padding)
	: GameObject(), direction(direction), windowSize(windowSize), padding(padding)
{
	// set offset
	switch (direction)
	{
	case Direction::LEFT:
		offset = Vector2f((float)windowSize.x - 2 * padding, 0.f);
		break;
	case Direction::RIGHT:
		offset = Vector2f(-((float)windowSize.x - 2 * padding), 0.f);
		break;
	case Direction::UP:
		offset = Vector2f(0.f, (float)windowSize.y - 2 * padding);
		break;
	case Direction::BOTTOM:
		offset = Vector2f(0.f, -((float)windowSize.y - 2 * padding));
		break;
	default:
		break;
	}

	Vector2f size, pos;
	switch (direction)
	{
	case Direction::LEFT:
		size = Vector2f(1.f, (float)windowSize.y);
		pos = Vector2f(padding, 0.f);
		break;
	case Direction::RIGHT:
		size = Vector2f(1.f, (float)windowSize.y);
		pos = Vector2f((float)windowSize.x - padding, 0.f);
		break;
	case Direction::UP:
		size = Vector2f((float)windowSize.x, 1.f);
		pos = Vector2f(0.f, padding);
		break;
	case Direction::BOTTOM:
		size = Vector2f((float)windowSize.x, 1.f);
		pos = Vector2f(0.f, (float)windowSize.y - padding);
		break;
	default:
		break;
	}

	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(this, ::Shape::RECTANGLE, ::Color::TRANS_PARENT, size, pos)
	);
	this->addGC(
		ComponentType::COLLIDABLE,
		new Collidable(
			this, 
			Collision::SIDEBOUNDARY,
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)),
			nullptr
		)
	);
}

void SideBoundary::updatePos(Vector2f offset)
{
	// move to the other direction by distance of offset 
	dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE))
		->getShape()->move(-offset);
}
