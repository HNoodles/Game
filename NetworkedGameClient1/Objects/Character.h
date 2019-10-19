#pragma once
#include <vector>
#include "GameObject.h"
#include "../Components/Collidable.h"
#include "MovingPlatform.h"
#include "SideBoundary.h"

class Character : public GameObject
{
private:
	Vector2f outVelocity;
	vector<Collidable*> boundary_ptrs;
	vector<Renderable*>* spawnPoints;

	bool hitBoundary;

	const Vector2f gravity = Vector2f(0.f, 500.f);
public:
	Character(
		::Shape shape, ::Color color, Vector2f size, Vector2f pos, 
		Vector2f velocity, Timeline& timeline, vector<Renderable*>* spawnPoints = nullptr
	);

	void handleKeyInput();

	void setOutVelocity(double elapsed);

	void checkHitBoundary(vector<SideBoundary*>* sideBoundaries);

	vector<Collidable*>* getBoundaryPtrs()
	{
		return &boundary_ptrs;
	}

	vector<Renderable*>* getSpawnPoints()
	{
		return spawnPoints;
	}

	Vector2f getOutVelocity() const
	{
		return outVelocity;
	}

	void setOutVelocity(Vector2f outVelocity)
	{
		this->outVelocity = outVelocity;
	}

	bool getHitBoundary() const
	{
		return hitBoundary;
	}

	void setHitBoundary(bool hitBoundary)
	{
		this->hitBoundary = hitBoundary;
	}
};