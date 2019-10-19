#pragma once
#include "GenericComponent.h"

enum class Shape {
	RECTANGLE, CIRCLE, DIAMOND
};

enum class Color {
	RED, GREEN, BLUE, TRANS_PARENT
};

class Renderable : public GenericComponent // define shape and color
{
private:
	sf::Shape* s;
	Vector2f& size;
	::Color color;
public:
	Renderable(GameObject* gameObject, ::Shape shape, ::Color color, Vector2f size, Vector2f pos);

	~Renderable();

	sf::Shape* getShape() 
	{
		return s;
	}

	Vector2f& getSize() const 
	{
		return size;
	}

	::Color getColor() const
	{
		return color;
	}
};

