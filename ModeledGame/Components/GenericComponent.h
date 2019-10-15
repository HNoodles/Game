#pragma once

#include <SFML/Graphics.hpp>
#include <stdarg.h>

using namespace sf;

enum class ComponentType {
	RENDERABLE, MOVABLE, COLLIDABLE
};

class GenericComponent // generic interface of game object components
{
public:
	virtual void work() 
	{
		return;
	}
};

