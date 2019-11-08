#include "../Character.h"

Character::Character(string id, EventManager* manager, 
	::Shape shape, ::Color color, Vector2f size, Vector2f pos,
	Vector2f velocity, Timeline& timeline, vector<SpawnPoint*>* spawnPoints)
	: GameObject(id, manager), outVelocity(0.f, 0.f), 
	boundary_ptrs({ nullptr, nullptr, nullptr, nullptr }), spawnPoints(spawnPoints)
{
	this->addGC(
		ComponentType::RENDERABLE, 
		new Renderable(this, shape, color, size, pos)
	);
	this->addGC(
		ComponentType::MOVABLE, 
		new Movable(
			this, 
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)), 
			velocity, timeline, Move::KEYINPUT
		)
	);
	this->addGC(
		ComponentType::COLLIDABLE, 
		new Collidable(
			this, 
			Collision::CHARACTER, 
			dynamic_cast<Renderable*>(this->getGC(ComponentType::RENDERABLE)), 
			dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))
		)
	);
}

void Character::handleKeyInput()
{
	Vector2f velocity = dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))->getVelocity();

	// calculate total velocity
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{// left
		outVelocity -= velocity;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{// right
		outVelocity += velocity;
	}
	if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		&& boundary_ptrs[3] != nullptr) // character should be on a platform to jump
	{// jump
		outVelocity.y = -300.f;
	}
}

void Character::setOutVelocity(double elapsed)
{
	Movable* movable = dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE));
	Vector2f velocity = movable->getVelocity();

	Collidable
		* left = boundary_ptrs[0],
		* right = boundary_ptrs[1],
		* up = boundary_ptrs[2],
		* bottom = boundary_ptrs[3];

	// clear out velocity to normal
	outVelocity.x = 0;

	if (left != nullptr)
	{// left collision!
		Movable* lmovable = left->getMovable();
		Vector2f lv = lmovable->getVelocity();
		outVelocity.x = dynamic_cast<, MovingPlatform*>(lmovable->getGameObject())
			->getHeadingPositive() ? lv.x : -lv.x;
	}

	if (right != nullptr)
	{// right collision!
		Movable* rmovable = right->getMovable();
		Vector2f rv = rmovable->getVelocity();
		outVelocity.x = dynamic_cast<, MovingPlatform*>(rmovable->getGameObject())
			->getHeadingPositive() ? rv.x : -rv.x;
	}

	if (up != nullptr)
	{// up collision!
		outVelocity.y = 0;
	}

	if (bottom != nullptr)
	{// bottom collision!
		Movable* bmovable = bottom->getMovable();
		Vector2f bv = bmovable->getVelocity();
		// only when no blocks on the heading side can character get velocity.x from bottom
		, MovingPlatform* bottomPlatform = dynamic_cast<, MovingPlatform*>(bmovable->getGameObject());
		if ((left == nullptr && !bottomPlatform->getHeadingPositive())
			|| (right == nullptr && bottomPlatform->getHeadingPositive()))
			outVelocity.x = bottomPlatform->getHeadingPositive() ? bv.x : -bv.x;
		outVelocity.y = bottomPlatform->getHeadingPositive() ? bv.y : -bv.y;
	}
	else // drop 
	{
		outVelocity.y += (float)(gravity.y * elapsed);
	}
}