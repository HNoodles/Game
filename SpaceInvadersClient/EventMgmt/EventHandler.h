#pragma once
#include "ECharDeath.h"
#include "EInvaDeath.h"
#include "ECharSpawn.h"
#include "EObjMovement.h"
#include "EUserInput.h"

//#include "../Scripting/ScriptingManager.h"

class EventManager;

class EventHandler
{
private:
	Timeline* gameTime;
	EventManager* manager;
	//ScriptingManager s_manager;

	void onCharDeath(ECharDeath e);

	void onInvaDeath(EInvaDeath e);

	void onCharSpawn(ECharSpawn e);

	void onObjMovement(EObjMovement e);

	void onUserInput(EUserInput e);
public:
	EventHandler(Timeline* gameTime, EventManager* manager);

	void onEvent(const ::Event* e);
};

