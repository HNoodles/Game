#include "../SideBoundary.h"

SideBoundary::SideBoundary(::Direction direction, Vector2f windowSize, float padding)
	: GameObject(), direction(direction), windowSize(windowSize), padding(padding)
{
	Vector2f size, pos;
	switch (direction)
	{
	case Direction::LEFT:
		size = Vector2f(padding, windowSize.y);
		pos = Vector2f(0.f, 0.f);
		break;
	case Direction::RIGHT:
		size = Vector2f(padding, windowSize.y);
		pos = Vector2f(windowSize.y - padding, 0.f);
		break;
	case Direction::UP:
		size = Vector2f(windowSize.x, padding);
		pos = Vector2f(0.f, 0.f);
		break;
	case Direction::BOTTOM:
		size = Vector2f(windowSize.x, padding);
		pos = Vector2f(0.f, windowSize.y - padding);
		break;
	default:
		break;
	}

	this->addGC(
		ComponentType::RENDERABLE,
		new Renderable(::Shape::RECTANGLE, ::Color::TRANSPARENT, size, pos)
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

Vector2f SideBoundary::getOffset() const
{
	Vector2f offset;

	switch (direction)
	{
	case Direction::LEFT:
		offset = Vector2f(windowSize.x - 2 * padding, 0.f);
		break;
	case Direction::RIGHT:
		offset = Vector2f( - windowSize.x + 2 * padding, 0.f);
		break;
	case Direction::UP:
		offset = Vector2f(0.f, windowSize.y - 2 * padding);
		break;
	case Direction::BOTTOM:
		offset = Vector2f(0.f, - windowSize.y + 2 * padding);
		break;
	default:
		break;
	}

	return offset;
}

void SideBoundary::updatePos(::Direction direction)
{
	// move to the other direction by distance of offset 
	dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE))
		->getShape()->move(-getOffset());
}
