#include "../Renderable.h"

Renderable::Renderable(::Shape shape, ::Color color, Vector2f& size, Vector2f& pos) 
	: size(size)
{
	// set shape
	switch (shape) {
	case RECT:
		s = &RectangleShape(size);
		break;
	case DIAMOND:
		ConvexShape* temp = new ConvexShape(4);
		temp->setPoint(0, Vector2f(size.x / 2, 0.f));
		temp->setPoint(1, Vector2f(size.x, size.y / 2));
		temp->setPoint(2, Vector2f(size.x / 2, size.y));
		temp->setPoint(3, Vector2f(0.f, size.y / 2));
		s = &ConvexShape(*temp);
		delete temp;
		break;
	case CIRCLE:
		s = &CircleShape(size.x / 2);
		break;
	default:
		s = &CircleShape();
		break;
	}

	// set pos
	s->setPosition(pos);
	
	// set color
	sf::Color c;
	switch (color) {
	case BLUE:
		c = sf::Color::Blue;
		break;
	case GREEN:
		c = sf::Color::Green;
		break;
	case RED:
		c = sf::Color::Red;
		break;
	}
	s->setFillColor(c);
}
