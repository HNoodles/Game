#include "../Renderable.h"

Renderable::Renderable(::Shape shape, ::Color color, Vector2f size, Vector2f pos) 
	: size(size)
{
	// set shape
	ConvexShape* temp = new ConvexShape(4);// used for DIAMOND
	switch (shape) {
	case ::Shape::RECTANGLE:
		s = &RectangleShape(size);
		break;
	case ::Shape::DIAMOND:
		temp->setPoint(0, Vector2f(size.x / 2, 0.f));
		temp->setPoint(1, Vector2f(size.x, size.y / 2));
		temp->setPoint(2, Vector2f(size.x / 2, size.y));
		temp->setPoint(3, Vector2f(0.f, size.y / 2));
		s = &ConvexShape(*temp);
		break;
	case ::Shape::CIRCLE:
		s = &CircleShape(size.x / 2);
		break;
	default:
		s = &CircleShape();
		break;
	}
	delete temp;


	// set pos
	s->setPosition(pos);
	
	// set color
	sf::Color c;
	switch (color) {
	case ::Color::BLUE:
		c = sf::Color::Blue;
		break;
	case ::Color::GREEN:
		c = sf::Color::Green;
		break;
	case ::Color::RED:
		c = sf::Color::Red;
		break;
	}
	s->setFillColor(c);
}
