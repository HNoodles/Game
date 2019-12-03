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
	vector<SpawnPoint*>* spawnPoints;

	const int MAX_BULLETS = 2;
	int bulletCount;
public:
	Character(
		string id, EventManager* manager, 
		::Shape shape, ::Color color, Vector2f size, Vector2f pos,
		Vector2f velocity, Timeline& timeline, vector<SpawnPoint*>* spawnPoints = nullptr
	);

	~Character();

	void handleKeyInput();

	void fire();

	Vector2f& getOutVelocity()
	{
		return outVelocity;
	}

	vector<SpawnPoint*>* getSpawnPoints()
	{
		return spawnPoints;
	}

	list<Collidable*> getBulletsList()
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
};