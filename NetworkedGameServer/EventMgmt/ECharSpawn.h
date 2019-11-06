#pragma once
#include "MyEvent.h"
#include "../Objects/Character.h"
#include "../Objects/SpawnPoint.h"

class ECharSpawn : public ::MyEvent
{
public:
	ECharSpawn(double executeTime, Character* character, SpawnPoint* spawnPoint);

	Character* getCharacter()
	{
		return (Character*)getArg(Content_t::CHARACTER_PTR).getVal(Variant_t::OBJ_PTR);
	}

	SpawnPoint* getSpawnPoint()
	{
		return (SpawnPoint*)getArg(Content_t::OBJ_PTR).getVal(Variant_t::OBJ_PTR);
	}
};

