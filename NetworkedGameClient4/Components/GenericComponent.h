#pragma once

#include <SFML/Graphics.hpp>
#include <stdarg.h>
//#include <iostream>

using namespace sf;
//using namespace std;

class GameObject;

enum class ComponentType {
	RENDERABLE, MOVABLE, COLLIDABLE
};

class GenericComponent // generic interface of game object components
{
protected:
	GameObject* gameObject;
public:
	GenericComponent(GameObject* gameObject)
	{
		this->gameObject = gameObject;
	}

	GameObject* getGameObject()
	{
		return gameObject;
	}

	virtual void work() 
	{
		return;
	}
};

