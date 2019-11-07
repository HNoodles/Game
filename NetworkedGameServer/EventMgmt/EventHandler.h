#pragma once
#include "ECharCollision.h"
#include "ECharDeath.h"
#include "ECharSpawn.h"
#include "EObjMovement.h"
#include "EUserInput.h"

class EventHandler
{
private:
	GameTime& gameTime;
	EventManager* manager;
	const char selfName;

	void onCharCollision(ECharCollision e);

	void platformWork(Character* character, MovingPlatform* platform);

	void deathZoneWork(Character* character);

	void sideBoundaryWork(SideBoundary* boundary);

	void onCharDeath(ECharDeath e);

	void onCharSpawn(ECharSpawn e);

	void onObjMovement(EObjMovement e);

	void onUserInput(EUserInput e);
public:
	EventHandler(GameTime& gameTime, EventManager* manager, const char selfName);

	void onEvent(const ::Event* e);
};

