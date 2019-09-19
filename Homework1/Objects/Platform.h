#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Platform : public RectangleShape
{
private:
public:
	Platform(Vector2f size);
};