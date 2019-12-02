#pragma once
#include <vector>
#include "GameObject.h"
#include "../Components/Collidable.h"

class Character : public GameObject
{
private:
	Vector2f outVelocity;
	vector<Collidable*> boundary_ptrs;
	vector<SpawnPoint*>* spawnPoints;

	bool hitBoundary;
	Vector2f* renderOffset;
	vector<SideBoundary*>* sideBoundaries;

	const Vector2f gravity = Vector2f(0.f, 500.f);
public:
	Character(
		string id, EventManager* manager, 
		::Shape shape, ::Color color, Vector2f size, Vector2f pos,
		Vector2f velocity, Timeline& timeline, vector<SpawnPoint*>* spawnPoints = nullptr, 
		Vector2f* renderOffset = nullptr, vector<SideBoundary*>* sideBoundaries = nullptr
	);

	void handleKeyInput();

	void setOutVelocity(double elapsed);

	void checkHitBoundary(vector<SideBoundary*>* sideBoundaries);

	vector<Collidable*>* getBoundaryPtrs()
	{
		return &boundary_ptrs;
	}

	vector<SpawnPoint*>* getSpawnPoints()
	{
		return spawnPoints;
	}

	Vector2f& getOutVelocity()
	{
		return outVelocity;
	}

	bool getHitBoundary() const
	{
		return hitBoundary;
	}

	void setHitBoundary(bool hitBoundary)
	{
		this->hitBoundary = hitBoundary;
	}

	Vector2f* getRenderOffset()
	{
		return renderOffset;
	}

	void setRenderOffset(Vector2f offset)
	{
		renderOffset->x = offset.x;
		renderOffset->y = offset.y;
	}

	vector<SideBoundary*>* getSideBoundaries()
	{
		return sideBoundaries;
	}
};