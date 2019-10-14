#include "../Collidable.h"

Collidable::Collidable(::Collision collision, Renderable* render) 
	: collision(collision), render(render)
{
}

void Collidable::work(list<Collidable*>& objects, list<Collidable*>& boundary_ptrs)
{
	// calculate four boundaries
	FloatRect box = render->getShape()->getGlobalBounds();
	Vector2f height(1.f, box.height), width(box.width, 1.f);

	RectangleShape l(height), r(height), u(width), b(width);
	l.setPosition(Vector2f(box.left, box.top));
	r.setPosition(Vector2f(box.left + box.width, box.top));
	u.setPosition(Vector2f(box.left, box.top));
	b.setPosition(Vector2f(box.left, box.top + box.height));

	// reset all bdry ptrs to null
	for (Collidable* ptr : boundary_ptrs)
		ptr = nullptr;

	for (Collidable* object : objects)
	{
		// get bound of object
		FloatRect bound = object->render->getShape()->getGlobalBounds();

		if (box.intersects(bound)) {// collision happens
			switch (object->collision)
			{
			case PLATFORM:
				// TODO: platform handler
				break;
			case DEADZONE: // implement in section 3
				break;
			case BOUNDARY: // implement in section 3
				break;
			default:
				break;
			}
		}
		

		// check collision and set out velocity
		if (l.getGlobalBounds().intersects(bound))
		{// left collision!
			left = platform;
		}
		if (r.getGlobalBounds().intersects(bound))
		{// right collision!
			right = platform;
		}
		if (u.getGlobalBounds().intersects(bound))
		{// up collision!
			up = platform;
		}
		if (b.getGlobalBounds().intersects(bound))
		{// bottom collision!
			bottom = platform;
		}
	}

	// set out velocity
	setOutVelocity(elapsed);
}
