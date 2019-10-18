#pragma once

#include <SFML/Graphics.hpp>
#include <stdarg.h>
//#include <iostream>

using namespace sf;
//using namespace std;

enum class ComponentType {
	RENDERABLE, MOVABLE, COLLIDABLE
};

class GenericComponent // generic interface of game object components
{
protected:
	void* gameObject;
public:
	GenericComponent(void* gameObject)
	{
		this->gameObject = gameObject;
	}

	virtual void work() 
	{
		return;
	}
};

