#pragma once
#include <vector>
#include "GameObject.h"
#include "../Components/Collidable.h"
#include "Bullet.h"

class Character : public GameObject
{
private:
	Vector2f outVelocity;

	list<Bullet*> bullets;
	list<Bullet*> expired;
	vector<SpawnPoint*>* spawnPoints;

	const unsigned int MAX_BULLETS = 2;
	const double MIN_FIRE_INTERVAL = 1;
	int bulletCount;
	float lastFireTime;
public:
	Character(
		string id, EventManager* manager, 
		::Shape shape, ::Color color, Vector2f size, Vector2f pos,
		Vector2f velocity, Timeline& timeline, vector<SpawnPoint*>* spawnPoints = nullptr
	);

	~Character();

	void handleKeyInput();

	void fire(double currentTime);

	void move(double elapsed);

	Vector2f& getOutVelocity()
	{
		return outVelocity;
	}

	vector<SpawnPoint*>* getSpawnPoints()
	{
		return spawnPoints;
	}

	list<Collidable*> getBulletsCList()
	{
		list<Collidable*> list;

		for (Bullet* bullet : bullets)
		{
			list.push_back(
				dynamic_cast<Collidable*>(bullet->getGC(ComponentType::COLLIDABLE))
			);
		}

		return list;
	}

	list<Renderable*> getRList()
	{
		list<Renderable*> list;

		// character itself
		list.push_back(
			dynamic_cast<Renderable*>(getGC(ComponentType::RENDERABLE))
		);
			
		for (Bullet* bullet : bullets)
		{
			list.push_back(
				dynamic_cast<Renderable*>(bullet->getGC(ComponentType::RENDERABLE))
			);
		}

		return list;
	}

	list<Bullet*>* getExpiredBullets()
	{
		return &expired;
	}

	void setSpawnPoints(vector<SpawnPoint*>* spawnPoints)
	{
		this->spawnPoints = spawnPoints;
	}
};