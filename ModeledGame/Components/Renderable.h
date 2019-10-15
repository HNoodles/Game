#pragma once
#include "GenericComponent.h"

enum class Shape {
	RECTANGLE, CIRCLE, DIAMOND
};

enum class Color {
	RED, GREEN, BLUE
};

class Renderable : public GenericComponent // define shape and color
{
private:
	sf::Shape* s;
	Vector2f& size;
public:
	Renderable(::Shape shape, ::Color color, Vector2f size, Vector2f pos);

	sf::Shape* getShape() 
	{
		return s;
	}

	Vector2f& getSize() 
	{
		return size;
	}
};

