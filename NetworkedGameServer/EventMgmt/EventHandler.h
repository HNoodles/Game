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

	void onCharCollision(ECharCollision e);

	void platformWork(Character* character, MovingPlatform* platform);

	void sideBoundaryWork(SideBoundary* boundary);

	void onCharDeath(ECharDeath e);

	void onCharSpawn(ECharSpawn e);

	void onObjMovement(EObjMovement e);

	void onUserInput(EUserInput e);
public:
	EventHandler(GameTime& gameTime);

	void onEvent(const ::Event* e);
};

