#pragma once

#include <SFML/Graphics.hpp>
#include <stdarg.h>

using namespace sf;

enum Type {
	DRAWABLE, MOVABLE, COLORABLE
};

class GenericComponent // generic interface of game object components
{
//protected:
//	enum::Type type;
public:
	/*enum::Type getType() {
		return type;
	}*/

	virtual void work() {
		return;
	}
};

