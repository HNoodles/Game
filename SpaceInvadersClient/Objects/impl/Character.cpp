#include "../Character.h"

Character::Character(string id, EventManager* manager, 
	::Shape shape, ::Color color, Vector2f size, Vector2f pos,
	Vector2f velocity, Timeline& timeline, vector<SpawnPoint*>* spawnPoints)
	: GameObject(id, manager), outVelocity(0.f, 0.f), 
	spawnPoints(spawnPoints), bulletCount(0), lastFireTime(-5)
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

Character::~Character()
{
	for (Bullet* bullet : bullets)
	{
		delete bullet;
		bullet = nullptr;
	}
	for (Bullet* bullet : expired)
	{
		delete bullet;
	}
}

void Character::handleKeyInput()
{
	Vector2f velocity = dynamic_cast<Movable*>(this->getGC(ComponentType::MOVABLE))->getVelocity();
	Keyboard::Key keyPressed = Keyboard::BackSpace;

	// get pressed key
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{// left
		keyPressed = Keyboard::A;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{// right
		keyPressed = Keyboard::D;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{// fire
		keyPressed = Keyboard::Space;
	}

	// generate event if there is any
	if (keyPressed != Keyboard::BackSpace)
		getEM()->insertEvent(new EUserInput(getEM()->getCurrentTime(), this, keyPressed));
	else // nothing pressed, reset out velocity
		outVelocity.x = 0;
}

void Character::fire(double currentTime)
{
	// check and remove expired bullets
	for (Bullet* bullet : expired)
	{
		delete bullet;
	}
	expired.clear();
	for (auto iter = bullets.begin(); iter != bullets.end(); )
	{
		Vector2f pos = dynamic_cast<Renderable*>((*iter)->getGC(ComponentType::RENDERABLE))
			->getShape()->getPosition();

		if (pos.y < 0) // out of screen
		{
			// move bullet to expired
			expired.push_back((*iter));
			bullets.erase(iter++);
		}
		else 
			iter++;
	}

	// limit maximum concurrent bullets and minimum fire interval
	if (bullets.size() >= MAX_BULLETS || (currentTime - lastFireTime) < MIN_FIRE_INTERVAL)
		return;

	// fire new bullet
	bulletCount++;
	lastFireTime = currentTime;
	Renderable* renderable = dynamic_cast<Renderable*>(getGC(ComponentType::RENDERABLE));
	Vector2f pos = renderable->getShape()->getPosition();
	Vector2f size = renderable->getSize();

	Vector2f bulletPos = Vector2f(pos.x + 1 / 2 * size.x, pos.y);

	Timeline& timeline = dynamic_cast<Movable*>(getGC(ComponentType::MOVABLE))->getTimeline();

	bullets.push_back(
		new Bullet(getId() + to_string(bulletCount), getEM(), bulletPos, timeline, false)
	);
}

void Character::move(double elapsed)
{
	// move character
	dynamic_cast<Movable*>(getGC(ComponentType::MOVABLE))->work(elapsed);

	// move bullets
	for (Bullet* bullet : bullets)
	{
		dynamic_cast<Movable*>(bullet->getGC(ComponentType::MOVABLE))->work(elapsed);
	}
}
