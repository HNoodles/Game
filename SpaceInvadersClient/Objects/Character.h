#pragma once
#include <vector>
#include "GameObject.h"
#include "../Components/Collidable.h"
#include "Bullet.h"

class Character : public GameObject
{
private:
	list<Bullet*> bullets;
	vector<Collidable*> boundary_ptrs;
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

	vector<Collidable*>* getBoundaryPtrs()
	{
		return &boundary_ptrs;
	}

	vector<SpawnPoint*>* getSpawnPoints()
	{
		return spawnPoints;
	}
};